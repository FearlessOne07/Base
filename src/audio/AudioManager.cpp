// Implementation
#include "base/audio/AudioManager.hpp"
#include "base/audio/signals/PlaySoundSignal.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Exception.hpp"
#include "internal/audio/SoundInstance.hpp"
#include "portaudio.h"
#include <algorithm>
#include <cstring>
#include <iostream>

namespace Base
{

  void AudioManager::Init()
  {
    // Initialize system audio device
    Pa_Initialize();

    PaStreamParameters outputParams{};
    outputParams.channelCount = 2;
    outputParams.sampleFormat = paFloat32;
    outputParams.hostApiSpecificStreamInfo = nullptr;

    std::vector<int> preferredRates = {48000, 44100, 96000};

#ifdef _WIN32
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
    outputParams.suggestedLatency = deviceInfo->defaultLowOutputLatency;

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
    int ApiIndex = Pa_GetDefaultHostApi();
    const PaHostApiInfo *info = Pa_GetHostApiInfo(ApiIndex);
    PaDeviceIndex deviceIndex = Pa_GetHostApiInfo(ApiIndex)->defaultOutputDevice;

    if (deviceIndex == paNoDevice)
    {
      THROW_BASE_RUNTIME_ERROR("No default PulseAudio output device.");
    }

    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(deviceIndex);
    outputParams.device = deviceIndex;
    outputParams.suggestedLatency = deviceInfo->defaultLowOutputLatency;

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

    PaError err = Pa_OpenStream(                                                                            //
      &_audioStream, nullptr, &outputParams, _sampleRate, FRAMES_PER_BUFFER, paClipOff, AudioCallBack, this //
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
      if (auto stopStreamSig = std::static_pointer_cast<StopAudioStreamSignal>(signal))
      {
        StopStream(stopStreamSig);
      }
    });
  }

  void AudioManager::SetAssetManager(AssetManager *assetManager)
  {
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
    float *out = static_cast<float *>(outputBuffer);
    AudioManager *_this = static_cast<AudioManager *>(userData);

    // Fill output buffer with silence
    memset(outputBuffer, 0, framesPerBuffer * 2 * sizeof(float));

    if (!_this->_audioRunning.load(std::memory_order_relaxed))
    {
      return paComplete;
    }

    // Process sound commands
    PlaySoundCommand soundCmd;
    while (_this->_soundCommands.Pop(soundCmd))
    {
      int slot = _this->FindFreeSoundSlot();
      if (slot >= 0)
      {
        soundCmd.instance->SetVolume(soundCmd.volume);
        soundCmd.instance->SetPan(soundCmd.pan);
        soundCmd.instance->Play();

        _this->_soundSlots[slot].instance = soundCmd.instance;
        _this->_soundSlots[slot].active.store(true, std::memory_order_release);
      }
    }

    // Process play stream commands
    PlayStreamCommand playStreamCmd;
    while (_this->_playStreamCommands.Pop(playStreamCmd))
    {
      int slot = _this->FindFreeStreamSlot();
      if (slot >= 0)
      {
        playStreamCmd.stream->SetVolume(playStreamCmd.volume);
        playStreamCmd.stream->SetPan(playStreamCmd.pan);
        playStreamCmd.stream->SetLoop(playStreamCmd.loop);
        playStreamCmd.stream->Play();

        _this->_streamSlots[slot].stream = playStreamCmd.stream;
        _this->_streamSlots[slot].active.store(true, std::memory_order_release);
      }
    }

    // Process stop stream commands
    StopStreamCommand stopStreamCmd;
    while (_this->_stopStreamCommands.Pop(stopStreamCmd))
    {
      int slot = _this->FindStreamSlot(stopStreamCmd.stream);
      if (slot >= 0)
      {
        _this->_streamSlots[slot].stream->Stop();
        _this->_streamSlots[slot].pendingRelease.store(true, std::memory_order_release);
      }
    }

    // Mix sounds
    for (size_t frame = 0; frame < framesPerBuffer; frame++)
    {
      for (size_t i = 0; i < MAX_SOUNDS; i++)
      {
        if (!_this->_soundSlots[i].active.load(std::memory_order_acquire))
        {
          continue;
        }

        SoundInstance *sound = _this->_soundSlots[i].instance;
        if (sound->IsPlaying())
        {
          auto soundFrame = sound->GetNextFrame();
          out[frame * 2 + 1] += soundFrame[1];
          out[frame * 2] += soundFrame[0];
        }
        else
        {
          // Mark for cleanup
          _this->_soundSlots[i].pendingRelease.store(true, std::memory_order_release);
        }
      }
    }

    // Mix streams
    for (size_t frame = 0; frame < framesPerBuffer; frame++)
    {
      for (size_t i = 0; i < MAX_STREAMS; i++)
      {
        if (!_this->_streamSlots[i].active.load(std::memory_order_acquire))
        {
          continue;
        }

        AudioStream *stream = _this->_streamSlots[i].stream;
        if (stream->IsPlaying())
        {
          auto streamFrame = stream->GetNextFrame();
          out[frame * 2] += streamFrame[0];
          out[frame * 2 + 1] += streamFrame[1];
        }
        else
        {
          // Mark for cleanup
          _this->_streamSlots[i].pendingRelease.store(true, std::memory_order_release);
        }
      }
    }

    // Clean up finished sounds/streams
    for (size_t i = 0; i < MAX_SOUNDS; i++)
    {
      if (_this->_soundSlots[i].pendingRelease.load(std::memory_order_acquire))
      {
        _this->_soundCleanupQueue.Push(_this->_soundSlots[i].instance);
        _this->_soundSlots[i].instance = nullptr;
        _this->_soundSlots[i].active.store(false, std::memory_order_release);
        _this->_soundSlots[i].pendingRelease.store(false, std::memory_order_release);
      }
    }

    for (size_t i = 0; i < MAX_STREAMS; i++)
    {
      if (_this->_streamSlots[i].pendingRelease.load(std::memory_order_acquire))
      {
        _this->_streamCleanupQueue.Push(_this->_streamSlots[i].stream);
        _this->_streamSlots[i].stream = nullptr;
        _this->_streamSlots[i].active.store(false, std::memory_order_release);
        _this->_streamSlots[i].pendingRelease.store(false, std::memory_order_release);
      }
    }
    return paContinue;
  }

  void AudioManager::PlaySound(const std::shared_ptr<PlaySoundSignal> &signal)
  {
    // Find a free slot in owned sounds
    int ownedSlot = -1;
    for (size_t i = 0; i < MAX_SOUNDS; i++)
    {
      if (!_ownedSounds[i].inUse)
      {
        ownedSlot = i;
        break;
      }
    }

    if (ownedSlot < 0)
    {
      return;
    }

    auto sound = signal->soundHandle.Get();
    auto instance = std::make_shared<SoundInstance>(sound);

    _ownedSounds[ownedSlot].instance = instance;
    _ownedSounds[ownedSlot].inUse = true;

    PlaySoundCommand cmd;
    cmd.instance = instance.get();
    cmd.volume = std::clamp(signal->soundVolume, 0.f, 1.f);
    cmd.pan = std::clamp(signal->soundPan, -1.f, 1.f);

    _soundCommands.Push(cmd);
  }

  void AudioManager::PlayStream(const std::shared_ptr<PlayAudioStreamSignal> &signal)
  {
    // Find a free slot in owned streams
    int ownedSlot = -1;
    for (size_t i = 0; i < MAX_STREAMS; i++)
    {
      if (!_ownedStreams[i].inUse)
      {
        ownedSlot = i;
        break;
      }
    }

    if (ownedSlot < 0)
    {
      return;
    }

    _ownedStreams[ownedSlot].handle = signal->streamHandle;
    _ownedStreams[ownedSlot].inUse = true;

    PlayStreamCommand cmd;
    cmd.stream = signal->streamHandle.Get().get();
    cmd.volume = std::clamp(signal->streamVolume, 0.f, 1.f);
    cmd.pan = std::clamp(signal->streamPan, -1.f, 1.f);
    cmd.loop = signal->loopStream;

    _playStreamCommands.Push(cmd);
  }

  void AudioManager::StopStream(const std::shared_ptr<StopAudioStreamSignal> &signal)
  {
    StopStreamCommand cmd;
    cmd.stream = signal->streamHandle.Get().get();
    _stopStreamCommands.Push(cmd);
  }

  bool AudioManager::AllSoundsDone()
  {
    for (size_t i = 0; i < MAX_SOUNDS; i++)
    {
      if (_soundSlots[i].active.load(std::memory_order_acquire))
      {
        return false;
      }
    }
    return true;
  }

  void AudioManager::ProcessCleanup()
  {
    // Clean up finished sounds
    SoundInstance *soundToClean;
    while (_soundCleanupQueue.Pop(soundToClean))
    {
      for (size_t i = 0; i < MAX_SOUNDS; i++)
      {
        if (_ownedSounds[i].inUse && _ownedSounds[i].instance.get() == soundToClean)
        {
          _ownedSounds[i].instance.reset();
          _ownedSounds[i].inUse = false;
          break;
        }
      }
    }

    // Clean up finished streams
    AudioStream *streamToClean;
    while (_streamCleanupQueue.Pop(streamToClean))
    {
      for (size_t i = 0; i < MAX_STREAMS; i++)
      {
        if (_ownedStreams[i].inUse && _ownedStreams[i].handle.Get().get() == streamToClean)
        {
          _ownedStreams[i].handle = AssetHandle<AudioStream>();
          _ownedStreams[i].inUse = false;
          break;
        }
      }
    }
  }

  void AudioManager::DeInit()
  {
    _audioRunning.store(false, std::memory_order_release);
    Pa_Sleep(20);

    if (Pa_IsStreamActive(_audioStream) == 1)
    {
      Pa_StopStream(_audioStream);
    }

    Pa_CloseStream(_audioStream);
    Pa_Terminate();
  }

  int AudioManager::FindFreeSoundSlot()
  {
    for (size_t i = 0; i < MAX_SOUNDS; i++)
    {
      if (!_soundSlots[i].active.load(std::memory_order_relaxed))
      {
        return i;
      }
    }
    return -1;
  }

  int AudioManager::FindFreeStreamSlot()
  {
    for (size_t i = 0; i < MAX_STREAMS; i++)
    {
      if (!_streamSlots[i].active.load(std::memory_order_relaxed))
      {
        return i;
      }
    }
    return -1;
  }

  int AudioManager::FindStreamSlot(AudioStream *stream)
  {
    for (size_t i = 0; i < MAX_STREAMS; i++)
    {
      if (_streamSlots[i].active.load(std::memory_order_relaxed) && _streamSlots[i].stream == stream)
      {
        return i;
      }
    }
    return -1;
  }

} // namespace Base
