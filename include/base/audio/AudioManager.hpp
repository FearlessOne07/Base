#pragma once
#include "Sound.hpp"
#include "base/assets/AssetManager.hpp"
#include <memory>
#include <portaudio.h>
#include <string>
#include <vector>

namespace Base
{
  class AudioManager
  {
  private:
    PaStream *_audioStream;
    std::vector<std::shared_ptr<Sound>> _sounds;
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
    void PlaySound(const std::string &soundName);
    bool AllSoundsDone();
    void DeInit();
  };
} // namespace Base
