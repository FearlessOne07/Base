#include "internal/audio/AudioManager.hpp"
#include "base/audio/signals/PlaySoundSignal.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Exception.hpp"
#include "portaudio.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>

namespace Base
{
  void AudioManager::Init()
  {
    // Initialize system audio device
    Pa_Initialize();

    PaStreamParameters outputParams{};
    outputParams.channelCount = 2;
    outputParams.sampleFormat = paInt16;
    outputParams.hostApiSpecificStreamInfo = nullptr;

    std::vector<int> preferredRates = {48000, 44100, 96000}; // Prefer 48k first

#ifdef _WIN32
    // Prefer WASAPI on Windows
    int wasapiApiIndex = -1;
    for (int i = 0; i < Pa_GetHostApiCount(); ++i)
    {
      const PaHostApiInfo *info = Pa_GetHostApiInfo(i);
      if (info->type == paWASAPI)
      {
        wasapiApiIndex = i;
        break;
      }
    }

    PaDeviceIndex deviceIndex = Pa_GetHostApiInfo(wasapiApiIndex)->defaultOutputDevice;

    if (deviceIndex == paNoDevice)
    {
      THROW_BASE_RUNTIME_ERROR("No default WASAPI output device.");
    }

    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(deviceIndex);
    outputParams.device = deviceIndex;
    outputParams.suggestedLatency = deviceInfo->defaultLowOutputLatency; // ← prioritize low latency

    bool supported = false;

    for (int rate : preferredRates)
    {
      if (Pa_IsFormatSupported(nullptr, &outputParams, rate) == paFormatIsSupported)
      {
        _sampleRate = rate;
        supported = true;
        break;
      }
    }

    if (!supported)
    {
      THROW_BASE_RUNTIME_ERROR("Neither 48000 Hz nor 44100 Hz supported with WASAPI and paInt16.");
    }
#elif defined(__linux__)

    // Preffer PulseAudio on LInux
    int pulseApiIndex = -1;
    for (int i = 0; i < Pa_GetHostApiCount(); ++i)
    {
      const PaHostApiInfo *info = Pa_GetHostApiInfo(i);
      if (info->type == paPulseAudio)
      {
        pulseApiIndex = i;
        break;
      }
    }

    PaDeviceIndex deviceIndex = Pa_GetHostApiInfo(pulseApiIndex)->defaultOutputDevice;

    if (deviceIndex == paNoDevice)
    {
      THROW_BASE_RUNTIME_ERROR("No default PulseAudio output device.");
    }

    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(deviceIndex);
    outputParams.device = deviceIndex;
    outputParams.suggestedLatency = deviceInfo->defaultLowOutputLatency; // ← prioritize low latency

    bool supported = false;

    for (int rate : preferredRates)
    {
      if (Pa_IsFormatSupported(nullptr, &outputParams, rate) == paFormatIsSupported)
      {
        _sampleRate = rate;
        supported = true;
        break;
      }
    }

    if (!supported)
    {
      THROW_BASE_RUNTIME_ERROR("Neither 48000 Hz nor 44100 Hz supported with PulseAudio and paInt16.");
    }

#endif

    // Open audio output stream
    PaError err = Pa_OpenStream(                                                             //
      &_audioStream, nullptr, &outputParams, _sampleRate, 64, paClipOff, AudioCallBack, this //
    );

    if (err != paNoError || Pa_StartStream(_audioStream) != paNoError)
    {
      THROW_BASE_RUNTIME_ERROR("Failed to initialize audio stream.");
    }

    std::cout << "Audio Stream opened: \n Sample Rate: " << _sampleRate << "\n";

    // Register Audio events
    auto bus = Base::SignalBus::GetInstance();
    bus->SubscribeSignal<PlaySoundSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto playSoundSig = std::static_pointer_cast<PlaySoundSignal>(signal))
      {
        PlaySound(playSoundSig);
      }
    });

    bus->SubscribeSignal<PlayAudioStreamSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto playStreamSig = std::static_pointer_cast<PlayAudioStreamSignal>(signal))
      {
        PlayStream(playStreamSig);
      }
    });

    bus->SubscribeSignal<StopAudioStreamSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto playStreamSig = std::static_pointer_cast<StopAudioStreamSignal>(signal))
      {
        StopStream(playStreamSig);
      }
    });
  }

  void AudioManager::SetAssetManager(AssetManager *assetManager)
  {
    // Set Asset manager to set audio load sample rate
    if (assetManager && !_assetManager)
    {
      _assetManager = assetManager;
      _assetManager->SetAudioSampleRate(_sampleRate);
    }
  }

  int AudioManager::AudioCallBack( //
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
    void *userData //
  )
  {
    int16_t *out = static_cast<int16_t *>(outputBuffer);
    AudioManager *_this = static_cast<AudioManager *>(userData);

    // Fill  output buffe with silence
    memset(outputBuffer, 0, framesPerBuffer * 2 * sizeof(int16_t));

    // Process queued sounds

    // Get Current Read index
    size_t read = _this->_soundReadIndex.load();
    while (read != _this->_soundWriteIndex.load(std::memory_order_acquire))
    {
      auto sound = _this->_pendingSounds[read];
      if (sound)
      {
        // Create Sound instance from sound
        _this->_sounds.emplace_back(std::move(sound));
        _this->_pendingSounds[read] = nullptr; // Clear the slot
      }

      // increment Read index
      read = (read + 1) % MAX_PENDING_SOUNDS;
    }
    _this->_soundReadIndex.store(read);

    // Proccess Queued Stream events
    read = _this->_streamReadIndex.load();
    while (read != _this->_streamWriteIndex.load(std::memory_order_acquire))
    {
      auto stream = _this->_pendingStreams[read];

      // If Stream isn't already playing
      if (stream && std::ranges::find(_this->_streams, stream) == _this->_streams.end())
      {
        // Add it to the list of playing streams
        _this->_streams.emplace_back(stream);
        _this->_pendingStreams[read] = AssetHandle<AudioStream>(); // Clear the slot
      }

      // Increment read index
      read = (read + 1) % MAX_PENDING_STREAMS;
    }
    _this->_streamReadIndex.store(read);

    // Higher Res mixing buffer
    std::vector<float> mixBuffer(framesPerBuffer * 2, 0);

    // Mix Sounds
    if (_this->_sounds.size() != 0)
    {
      // For every audio frame
      for (size_t frame = 0; frame < framesPerBuffer; frame++)
      {
        // Mix sound instances
        for (auto it = _this->_sounds.begin(); it != _this->_sounds.end();)
        {
          std::shared_ptr<SoundInstance> &sound = *it;

          // If sound is still playing
          if (sound->IsPlaying())
          {
            // Get next audio frame
            auto soundFrame = sound->GetNextFrame();

            // Mix it in
            mixBuffer[frame * 2] += soundFrame[0];
            mixBuffer[frame * 2 + 1] += soundFrame[1];
            it++;
          }
          else
          {
            // If not, erase the sound form the list
            it = _this->_sounds.erase(it);
          }
        }
      }
    }

    // mix Streams
    if (_this->_streams.size() != 0)
    {
      // For every audio frame
      for (size_t frame = 0; frame < framesPerBuffer; frame++)
      {
        for (auto it = _this->_streams.begin(); it != _this->_streams.end();)
        {
          std::shared_ptr<AudioStream> stream = it->Get();

          // if Stream is still playing
          if (stream->IsPlaying())
          {
            // Get next frame
            auto streamFrame = stream->GetNextFrame();

            // Mix it in
            mixBuffer[frame * 2] += streamFrame[0];
            mixBuffer[frame * 2 + 1] += streamFrame[1];
            it++;
          }
          else
          {
            // Stream isn't playing; remove it
            it = _this->_streams.erase(it);
          }
        }
      }
    }

    // Convert back to 16-bit with clipping protection
    for (size_t i = 0; i < framesPerBuffer * 2; i++)
    {
      out[i] = static_cast<int16_t>(std::clamp<float>(mixBuffer[i], -1.0, 1.0) * 32767.0f);
    }

    // Continue
    return paContinue;
  }

  void AudioManager::PlaySound(const std::shared_ptr<PlaySoundSignal> &signal)
  {
    // Get wirte index
    size_t write = _soundWriteIndex.load();
    size_t nextWrite = (write + 1) % MAX_PENDING_SOUNDS;

    // if sound queue isn't full
    if (nextWrite != _soundReadIndex.load())
    {
      // Get the sound asset
      auto sound = signal->soundHandle.Get();

      // Create the sound instance
      std::shared_ptr<SoundInstance> instance = std::make_shared<SoundInstance>(sound);

      // Set pan / volume settings
      instance->SetVolume(std::clamp(signal->soundVolume, 0.f, 1.f));
      instance->SetPan(std::clamp(signal->soundPan, -1.f, 1.f));

      // Mark the sound as playing
      instance->Play();

      // add it to pending sounds list
      _pendingSounds[write] = std::move(instance);

      // Store next index
      _soundWriteIndex.store(nextWrite, std::memory_order_release);
    }
  }

  void AudioManager::PlayStream(const std::shared_ptr<PlayAudioStreamSignal> &signal)
  {
    size_t write = _streamWriteIndex.load();
    size_t nextWrite = (write + 1) % MAX_PENDING_STREAMS;

    // if stream queue isn't full
    if (nextWrite != _streamReadIndex.load())
    {
      // Get the stream asset
      auto stream = signal->streamHandle.Get();

      // Set stream pan / loop / volume settings
      stream->SetVolume(std::clamp(signal->streamVolume, 0.f, 1.f));
      stream->SetPan(std::clamp(signal->streamPan, -1.f, 1.f));
      stream->SetLoop(signal->loopStream);

      // Marks stream as playing
      stream->Play();

      // Add stream to queue
      _pendingStreams[write] = std::move(signal->streamHandle);

      // update stream write index
      _streamWriteIndex.store(nextWrite, std::memory_order_release);
    }
  }

  void AudioManager::StopStream(const std::shared_ptr<StopAudioStreamSignal> &signal)
  {
    AssetHandle<AudioStream> handle = signal->streamHandle;
    if (auto it = std::ranges::find(_streams, handle); it != _streams.end())
    {
      it->Get()->Stop();
      _streams.erase(it);
    }
  }

  bool AudioManager::AllSoundsDone()
  {
    return _sounds.empty();
  }

  void AudioManager::DeInit()
  {
    // Stop and close output stream
    Pa_StopStream(_audioStream);
    Pa_CloseStream(_audioStream);
    Pa_Terminate();
  }
} // namespace Base
