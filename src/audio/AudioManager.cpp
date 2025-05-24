
#include "internal/audio/AudioManager.hpp"
#include "base/audio/signals/PlaySoundSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Exception.hpp"
#include <algorithm>
#include <cstring>
#include <memory>
namespace Base
{
  void AudioManager::Init()
  {
    Pa_Initialize();

    PaStreamParameters outputParams{};
    outputParams.channelCount = 2;
    outputParams.sampleFormat = paInt16;
    outputParams.hostApiSpecificStreamInfo = nullptr;

#ifdef _WIN32
    // Use WASAPI on Windows
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

    if (wasapiApiIndex == -1)
    {
      THROW_BASE_RUNTIME_ERROR("WASAPI not available.");
    }

    PaDeviceIndex deviceIndex = Pa_GetHostApiInfo(wasapiApiIndex)->defaultOutputDevice;
#else
    // Use default output device on Linux/macOS
    PaDeviceIndex deviceIndex = Pa_GetDefaultOutputDevice();
#endif

    if (deviceIndex == paNoDevice)
    {
      THROW_BASE_RUNTIME_ERROR("No default output device.");
    }

    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(deviceIndex);
    outputParams.device = deviceIndex;
    outputParams.suggestedLatency = deviceInfo->defaultLowOutputLatency;

    PaError err = Pa_OpenStream(&_audioStream, nullptr, &outputParams, 44100,
                                64, // adjust for latency
                                paClipOff, AudioCallBack, this);

    if (err != paNoError || Pa_StartStream(_audioStream) != paNoError)
    {
      THROW_BASE_RUNTIME_ERROR("Failed to initialize audio stream.");
    }

    // Register Events
    auto bus = Base::SignalBus::GetInstance();
    bus->SubscribeSignal<PlaySoundSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto playSoundSig = std::static_pointer_cast<PlaySoundSignal>(signal))
      {
        PlaySound(playSoundSig);
      }
    });
  }

  void AudioManager::SetAssetManager(AssetManager *assetManager)
  {
    if (assetManager)
    {
      _assetManager = assetManager;
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

    memset(outputBuffer, 0, framesPerBuffer * 2 * sizeof(int16_t));

    // Process queued sounds
    size_t read = _this->_readIndex.load();
    while (read != _this->_writeIndex.load(std::memory_order_acquire))
    {
      auto sound = _this->_pendingSounds[read];
      if (sound)
      {
        _this->_sounds.emplace_back(std::move(sound));
        _this->_pendingSounds[read] = nullptr; // Clear the slot
      }
      read = (read + 1) % MAX_PENDING_SOUNDS;
    }
    _this->_readIndex.store(read);

    if (_this->_sounds.size() != 0)
    {
      for (size_t frame = 0; frame < framesPerBuffer; frame++)
      {
        for (auto it = _this->_sounds.begin(); it != _this->_sounds.end();)
        {
          std::shared_ptr<SoundInstance> &sound = *it;
          if (sound->IsPlaying())
          {
            auto soundFrame = sound->GetNextFrame();
            out[frame * 2] += soundFrame[0];
            out[frame * 2 + 1] += soundFrame[1];
            it++;
          }
          else
          {
            it = _this->_sounds.erase(it);
          }
        }
      }
    }
    return paContinue;
  }

  void AudioManager::PlaySound(const std::shared_ptr<PlaySoundSignal> &signal)
  {
    size_t write = _writeIndex.load();
    size_t nextWrite = (write + 1) % MAX_PENDING_SOUNDS;

    if (nextWrite != _readIndex.load())
    {
      auto sound = _assetManager->GetAsset<Sound>(signal->soundName);
      std::shared_ptr<SoundInstance> instance = std::make_shared<SoundInstance>(sound);
      instance->SetVolume(std::clamp(signal->soundVolume, 0.f, 1.f));
      instance->SetPan(std::clamp(signal->soundPan, -1.f, 1.f));
      instance->Play();
      _pendingSounds[write] = std::move(instance);
      _writeIndex.store(nextWrite, std::memory_order_release);
    }
  }

  bool AudioManager::AllSoundsDone()
  {
    return _sounds.empty();
  }

  void AudioManager::DeInit()
  {
    Pa_StopStream(_audioStream);
    Pa_CloseStream(_audioStream);
    Pa_Terminate();
  }
} // namespace Base
