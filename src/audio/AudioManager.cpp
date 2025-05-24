
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
    Pa_OpenDefaultStream( //
      &_audioStream, 0, 2, paInt16, 44100, 256, AudioManager::AudioCallBack,
      this //
    );

    if (Pa_StartStream(_audioStream) != paNoError)
    {
      THROW_BASE_RUNTIME_ERROR("Failed to initialise audio stream");
    }

    // Resgister Events
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
      auto signal = _this->_pendingSignals[read];
      if (signal)
      {
        auto sound = _this->_assetManager->GetAsset<Sound>(signal->soundName);
        SoundInstance instance(sound);
        instance.SetVolume(std::clamp(signal->soundVolume, 0.f, 1.f));
        instance.SetPan(std::clamp(signal->soundPan, -1.f, 1.f));
        instance.Play();
        _this->_sounds.emplace_back(std::move(instance));

        _this->_pendingSignals[read] = nullptr; // Clear the slot
      } 
      read = (read + 1) % MAX_PENDING_SIGNALS;
    }
    _this->_readIndex.store(read);



    if (_this->_sounds.size() != 0)
    {
      for (size_t frame = 0; frame < framesPerBuffer; frame++)
      {
        for (auto it = _this->_sounds.begin(); it != _this->_sounds.end();)
        {
          SoundInstance &sound = *it;
          if (sound.IsPlaying())
          {
            auto soundFrame = sound.GetNextFrame();
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
    size_t nextWrite = (write + 1) % MAX_PENDING_SIGNALS;

    if (nextWrite != _readIndex.load())
    {
      _pendingSignals[write] = signal;
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
