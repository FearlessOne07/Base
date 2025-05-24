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
    std::vector<std::shared_ptr<SoundInstance>> _sounds;
    AssetManager *_assetManager;

    static constexpr int MAX_PENDING_SOUNDS = 64; // Maximum number of pending signals
    std::array<std::shared_ptr<SoundInstance>, MAX_PENDING_SOUNDS> _pendingSounds;
    std::atomic<int> _readIndex;
    std::atomic<int> _writeIndex;

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
