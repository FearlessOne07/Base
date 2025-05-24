
#include "base/audio/AudioManager.hpp"
#include "base/audio/signals/PlaySoundSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Exception.hpp"
#include <cstring>
#include <memory>
#include <string>
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
        PlaySound(playSoundSig->soundName);
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

    if (_this->_sounds.size() != 0)
    {
      for (size_t frame = 0; frame < framesPerBuffer; frame++)
      {
        for (auto it = _this->_sounds.begin(); it != _this->_sounds.end();)
        {
          std::shared_ptr<Sound> &sound = *it;
          if (sound->IsPlaying())
          {
            auto soundFrame = sound->GetNextFrame();
            out[frame * 2] += soundFrame[0];
            out[frame * 2 + 1] += soundFrame[1];
            it++;
          }
          else
          {
            out[frame * 2] = 0;
            out[frame * 2 + 1] = 0;
            sound->Reset();
            it = _this->_sounds.erase(it);
          }
        }
      }
    }
    return paContinue;
  }

  void AudioManager::PlaySound(const std::string &soundName)
  {
    std::shared_ptr<Sound> sound = _assetManager->GetAsset<Sound>(soundName);
    if (std::ranges::find(_sounds, sound) == _sounds.end())
    {
      sound->Play();
      _sounds.push_back(sound);
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
