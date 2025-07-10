#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/audio/signals/PlaySoundSignal.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "internal/audio/SoundInstance.hpp"
#include <atomic>
#include <portaudio.h>
#include <vector>

namespace Base
{
  class AudioManager
  {
  private:
    PaStream *_audioStream;
    AssetManager *_assetManager;
    uint64_t _sampleRate = 96000;

    std::vector<std::shared_ptr<SoundInstance>> _sounds;
    std::vector<AssetHandle<AudioStream>> _streams;

    static constexpr int MAX_PENDING_SOUNDS = 64; // Maximum number of pending signals
    std::array<std::shared_ptr<SoundInstance>, MAX_PENDING_SOUNDS> _pendingSounds;
    std::atomic<int> _soundReadIndex;
    std::atomic<int> _soundWriteIndex;

    static constexpr int MAX_PENDING_STREAMS = 8; // Maximum number of pending signals
    std::array<AssetHandle<AudioStream>, MAX_PENDING_STREAMS> _pendingStreams;
    std::atomic<int> _streamReadIndex;
    std::atomic<int> _streamWriteIndex;

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
    void PlayStream(const std::shared_ptr<PlayAudioStreamSignal> &signal);
    void StopStream(const std::shared_ptr<StopAudioStreamSignal> &signal);
    bool AllSoundsDone();
    void DeInit();
  };
} // namespace Base
