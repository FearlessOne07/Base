#pragma once
#include "base/assets/BaseAsset.hpp"
#include <array>
#include <atomic>
#include <cstdint>
#include <miniaudio.h>
#include <mutex>
#include <samplerate.h>
#include <thread>
#include <vector>

namespace Base
{
  class AudioStream : public BaseAsset
  {
  private:
    ma_decoder _decoder;
    SRC_STATE *_srcState;
    SRC_DATA _srcData;
    uint64_t _dataBufferFrameCount = 2048;
    uint64_t _defaultSampleRate = 0;
    uint64_t _targetSampleRate = 0;
    double _srcRatio = 1.0;
    uint64_t _outputFrameCount = 0;

    // Double buffering
    std::vector<float> _resampledBuffers[2];
    std::vector<float> _toResampleBuffer;

    // Thread synchronization
    std::atomic<bool> _fillBuffers{false};
    std::atomic<bool> _isBufferReady[2]{false, false};
    std::thread _bufferThread;
    std::mutex _mutex;

    // Buffer management
    int _currentBuffer = 0; // Which buffer we're currently reading from
    uint64_t _currentFrame = 0;
    uint64_t _resampledFrameCount = 0;
    uint64_t _bufferedFrameCount[2] = {0, 0}; // How many frames in each buffer

    // Effects
    float _volume = 1.0f;
    float _pan = 0.0f;

    // State
    bool _isPlaying = false;
    bool _loop = false;
    bool _lastBufferRound = false;

  private:
    void FillBuffers();

  public:
    AudioStream(ma_decoder decoder, uint64_t defaultSampleRate, uint64_t targetSampleRate);
    ~AudioStream();

    std::array<float, 2> GetNextFrame();
    void SetVolume(float volume);
    void SetPan(float pan);
    void SetLoop(bool loop);
    void Play();
    void Pause();
    void Stop();
    void Resume();
    float GetVolume() const;
    float GetPan() const;
    bool IsPlaying() const;
  };
} // namespace Base
