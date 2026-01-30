
#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/audio/signals/PlaySoundSignal.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "internal/utils/TSCommandQueue.hpp"
#include <array>
#include <atomic>
#include <portaudio.h>

namespace Base
{
  // RT-safe sound slot that holds raw pointers and state
  class SoundInstance;
  struct SoundSlot
  {
    SoundInstance *instance = nullptr;
    std::atomic<bool> active{false};
    std::atomic<bool> pendingRelease{false};
  };

  // RT-safe stream slot
  struct StreamSlot
  {
    AudioStream *stream = nullptr;
    std::atomic<bool> active{false};
    std::atomic<bool> pendingRelease{false};
  };

  // Commands for RT thread
  struct PlaySoundCommand
  {
    SoundInstance *instance;
    float volume;
    float pan;
  };

  struct PlayStreamCommand
  {
    AudioStream *stream;
    float volume;
    float pan;
    bool loop;
  };

  struct StopStreamCommand
  {
    AudioStream *stream;
  };

  class AudioManager
  {
  private:
    PaStream *_audioStream = nullptr;
    AssetManager *_assetManager = nullptr;
    uint64_t _sampleRate = 96000;
    std::atomic<bool> _audioRunning{true};

    // RT-safe sound management
    static constexpr size_t MAX_SOUNDS = 64;
    std::array<SoundSlot, MAX_SOUNDS> _soundSlots;
    TSCommandQueue<PlaySoundCommand, MAX_SOUNDS> _soundCommands;

    // RT-safe stream management
    static constexpr size_t MAX_STREAMS = 8;
    std::array<StreamSlot, MAX_STREAMS> _streamSlots;
    TSCommandQueue<PlayStreamCommand, MAX_STREAMS> _playStreamCommands;
    TSCommandQueue<StopStreamCommand, MAX_STREAMS> _stopStreamCommands;

    // Cleanup queue
    TSCommandQueue<SoundInstance *, MAX_SOUNDS> _soundCleanupQueue;
    TSCommandQueue<AudioStream *, MAX_STREAMS> _streamCleanupQueue;

    static constexpr int FRAMES_PER_BUFFER = 64 * 8;

    // Storage for owned instances
    struct OwnedSound
    {
      std::shared_ptr<SoundInstance> instance;
      bool inUse = false;
    };
    std::array<OwnedSound, MAX_SOUNDS> _ownedSounds;

    struct OwnedStream
    {
      AssetHandle<AudioStream> handle;
      bool inUse = false;
    };
    std::array<OwnedStream, MAX_STREAMS> _ownedStreams;

  public:
    static int AudioCallBack( //
      const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
      const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
      void *userData //
    );

    void Init();
    void SetAssetManager(AssetManager *assetManager);
    void PlaySound(const std::shared_ptr<PlaySoundSignal> &signal);
    void PlayStream(const std::shared_ptr<PlayAudioStreamSignal> &signal);
    void StopStream(const std::shared_ptr<StopAudioStreamSignal> &signal);
    bool AllSoundsDone();
    void ProcessCleanup();
    void DeInit();

  private:
    int FindFreeSoundSlot();
    int FindFreeStreamSlot();
    int FindStreamSlot(AudioStream *stream);
  };

} // namespace Base
