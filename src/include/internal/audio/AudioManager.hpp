#pragma once
#include "base/assets/AssetManager.hpp"
#include "base/audio/signals/PlaySoundSignal.hpp"
#include "internal/audio/SoundInstance.hpp"
#include <portaudio.h>
#include <vector>

namespace Base
{
  class AudioManager
  {
  private:
    PaStream *_audioStream;
    std::vector<SoundInstance> _sounds;
    AssetManager *_assetManager;

  public:
    static int AudioCallBack( //
      const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
      const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
      void *userData //
    );

  public:
    void Init();
    void SetAssetManager(AssetManager *assetManager);
    void PlaySound(const std::shared_ptr<PlaySoundSignal> &signal);
    bool AllSoundsDone();
    void DeInit();
  };
} // namespace Base
