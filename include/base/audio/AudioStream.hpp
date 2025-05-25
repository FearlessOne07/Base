#pragma once
#include <array>
#include <cstdint>
#include <miniaudio.h>
#include <vector>

namespace Base
{
  class AudioStream
  {
  private:
    ma_decoder _decoder;
    ma_resampler _resampler;

    std::vector<int16_t> _resampledBuffer;
    std::vector<int16_t> _toResampleBuffer;
    uint64_t _dataBufferFrameCount = 512;
    uint64_t _dataBufferSize = _dataBufferFrameCount * 2;

    uint64_t _defaultSampleRate = 0;
    uint64_t _targetSampleRate = 0;
    // Frame
    uint64_t _currentFrame = 0;

    // Effects
    float _volume = 1.f;
    float _pan = 0.5;

    // State
    bool _isPlaying = false;
    bool _loop = false;

    bool _lastBufferRound = false;

  public:
    AudioStream(ma_decoder decoder, uint64_t defaultSampleRate, uint64_t targetSampleRate);
    std::array<int16_t, 2> GetNextFrame();

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
