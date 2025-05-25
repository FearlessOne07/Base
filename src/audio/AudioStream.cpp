#include "base/audio/AudioStream.hpp"
#include "base/util/Exception.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace Base
{
  AudioStream::AudioStream(ma_decoder decoder, uint64_t defaultSampleRate, uint64_t targetSampleRate)
    : _decoder(decoder), _defaultSampleRate(defaultSampleRate), _targetSampleRate(targetSampleRate)
  {
    _resampledBuffer.resize(_dataBufferFrameCount * 2);
    _toResampleBuffer.resize(_dataBufferFrameCount * 2);

    // Initialize resampler config
    ma_resampler_config resamplerConfig = ma_resampler_config_init(       //
      ma_format_s16,                                                      // sample format
      2,                                                                  // channels (stereo)
      _defaultSampleRate, _targetSampleRate, ma_resample_algorithm_linear //
    );

    // Initialize resampler
    if (ma_resampler_init(&resamplerConfig, nullptr, &_resampler) != MA_SUCCESS)
    {
      THROW_BASE_RUNTIME_ERROR("Failed to initialize resampler");
    }
  }

  std::array<int16_t, 2> AudioStream::GetNextFrame()
  {
    std::array<int16_t, 2> frame;
    if (_currentFrame >= _dataBufferFrameCount)
    {
      if (_lastBufferRound)
      {
        frame[0] = 0;
        frame[1] = 0;
        _isPlaying = false;
        _currentFrame = 0;
        _lastBufferRound = false;
        return frame;
      }

      memset(_resampledBuffer.data(), 0, _dataBufferFrameCount * 2 * sizeof(int16_t));
      memset(_toResampleBuffer.data(), 0, _dataBufferFrameCount * 2 * sizeof(int16_t));

      _currentFrame = 0;
      uint64_t readFrames = 0;

      ma_decoder_read_pcm_frames(&_decoder, _toResampleBuffer.data(), _dataBufferFrameCount, (ma_uint64 *)&readFrames);

      if (readFrames < _dataBufferFrameCount)
      {
        if (!_loop)
        {
          _lastBufferRound = true;
        }
        else
        {
          uint64_t framesToRead = _dataBufferFrameCount - readFrames;
          ma_decoder_seek_to_pcm_frame(&_decoder, 0);
          ma_decoder_read_pcm_frames(                                                   //
            &_decoder, _toResampleBuffer.data() + readFrames * 2, framesToRead, nullptr //
          );
        }
      }
    }

    ma_uint64 toResamlpe = _dataBufferFrameCount;

    ma_resampler_process_pcm_frames(                                                           //
      &_resampler, _toResampleBuffer.data(), &toResamlpe, _resampledBuffer.data(), &toResamlpe //
    );

    frame = {_resampledBuffer[_currentFrame * 2], _resampledBuffer[_currentFrame * 2 + 1]};
    float angle = _pan * (PI / 2.f);
    float leftPan = std::cos(angle);
    float rightPan = std::sin(angle);

    frame[0] = frame[0] * _volume * leftPan;
    frame[1] = frame[1] * _volume * rightPan;

    _currentFrame++;
    return frame;
  }

  void AudioStream::SetVolume(float volume)
  {
    _volume = volume;
    _volume = std::clamp<float>(_volume, 0.f, 1.f);
  }

  void AudioStream::SetPan(float pan)
  {
    _pan = pan;
    _pan = std::clamp<float>(_pan, -1.f, 1.f);
  }

  void AudioStream::SetLoop(bool loop)
  {
    _loop = loop;
  }

  void AudioStream::Play()
  {
    _isPlaying = true;
    _lastBufferRound = false;
  }

  void AudioStream::Pause()
  {
    _isPlaying = false;
  }

  void AudioStream::Resume()
  {
    _isPlaying = true;
  }

  void AudioStream::Stop()
  {
    _isPlaying = false;
    _lastBufferRound = false;
    _currentFrame = 0;
    ma_decoder_seek_to_pcm_frame(&_decoder, 0);
  }

  float AudioStream::GetVolume() const
  {
    return _volume;
  }
  float AudioStream::GetPan() const
  {
    return _pan;
  }

  bool AudioStream::IsPlaying() const
  {
    return _isPlaying;
  }
} // namespace Base
