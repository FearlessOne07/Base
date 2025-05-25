#include "base/audio/AudioStream.hpp"
#include "raylib.h"
#include <algorithm>
#include <cstdint>
#include <cstring>

namespace Base
{
  AudioStream::AudioStream(ma_decoder decoder) : _decoder(decoder)
  {
    _dataBuffer.resize(_dataBufferFrameCount * 2);
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

      memset(_dataBuffer.data(), 0, _dataBufferFrameCount * 2 * sizeof(int16_t));
      _currentFrame = 0;
      uint64_t readFrames = 0;
      ma_decoder_read_pcm_frames(&_decoder, _dataBuffer.data(), _dataBufferFrameCount, (ma_uint64 *)&readFrames);

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
          ma_decoder_read_pcm_frames(                                                    //
            &_decoder, _dataBuffer.data() + framesToRead, _dataBufferFrameCount, nullptr //
          );
        }
      }
    }

    frame = {_dataBuffer[_currentFrame * 2], _dataBuffer[_currentFrame * 2 + 1]};
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
