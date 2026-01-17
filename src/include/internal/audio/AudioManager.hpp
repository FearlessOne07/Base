#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/audio/signals/PlaySoundSignal.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "internal/audio/SoundInstance.hpp"
#include <array>
#include <atomic>
#include <portaudio.h>

namespace Base
{
  // RT-safe sound slot that holds raw pointers and state
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

  // Lock-free command queue for audio operations
  template <typename T, size_t Size> class RTCommandQueue
  {
  private:
    struct Command
    {
      T data;
      std::atomic<bool> ready{false};
    };

    std::array<Command, Size> _commands;
    std::atomic<size_t> _writeIndex{0};
    std::atomic<size_t> _readIndex{0};

  public:
    bool Push(const T &item)
    {
      size_t write = _writeIndex.load(std::memory_order_relaxed);
      size_t nextWrite = (write + 1) % Size;

      if (nextWrite == _readIndex.load(std::memory_order_acquire))
      {
        return false; // Queue full
      }

      _commands[write].data = item;
      _commands[write].ready.store(true, std::memory_order_release);
      _writeIndex.store(nextWrite, std::memory_order_release);
      return true;
    }

    bool Pop(T &item)
    {
      size_t read = _readIndex.load(std::memory_order_relaxed);

      if (!_commands[read].ready.load(std::memory_order_acquire))
      {
        return false; // Nothing to read
      }

      item = _commands[read].data;
      _commands[read].ready.store(false, std::memory_order_release);
      _readIndex.store((read + 1) % Size, std::memory_order_release);
      return true;
    }
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
    RTCommandQueue<PlaySoundCommand, MAX_SOUNDS> _soundCommands;

    // RT-safe stream management
    static constexpr size_t MAX_STREAMS = 8;
    std::array<StreamSlot, MAX_STREAMS> _streamSlots;
    RTCommandQueue<PlayStreamCommand, MAX_STREAMS> _playStreamCommands;
    RTCommandQueue<StopStreamCommand, MAX_STREAMS> _stopStreamCommands;

    // Cleanup queue (non-RT thread reads this)
    RTCommandQueue<SoundInstance *, MAX_SOUNDS> _soundCleanupQueue;
    RTCommandQueue<AudioStream *, MAX_STREAMS> _streamCleanupQueue;

    static constexpr int FRAMES_PER_BUFFER = 64 * 8;

    // Storage for owned instances (accessed only from non-RT thread)
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
    static int AudioCallBack(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                             void *userData);

    void Init();
    void SetAssetManager(AssetManager *assetManager);
    void PlaySound(const std::shared_ptr<PlaySoundSignal> &signal);
    void PlayStream(const std::shared_ptr<PlayAudioStreamSignal> &signal);
    void StopStream(const std::shared_ptr<StopAudioStreamSignal> &signal);
    bool AllSoundsDone();
    void ProcessCleanup(); // Call periodically from non-RT thread
    void DeInit();

  private:
    int FindFreeSoundSlot();
    int FindFreeStreamSlot();
    int FindStreamSlot(AudioStream *stream);
  };

} // namespace Base
