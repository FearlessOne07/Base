#include "base/audio/AudioStream.hpp"
#include "base/util/Exception.hpp"
#include "samplerate.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <thread>

namespace Base
{
  AudioStream::AudioStream(ma_decoder decoder, uint64_t defaultSampleRate, uint64_t targetSampleRate)
    : _decoder(decoder), _defaultSampleRate(defaultSampleRate), _targetSampleRate(targetSampleRate)
  {
    // Calculate sample rate conversion ratio
    _srcRatio = static_cast<double>(_targetSampleRate) / static_cast<double>(_defaultSampleRate);
    _outputFrameCount = static_cast<uint64_t>(_srcRatio * _dataBufferFrameCount + 256); // Add padding for safety

    // Initialize libsamplerate converter
    int error;
    _srcState = src_new(SRC_SINC_BEST_QUALITY, 2, &error); // 2 channels (stereo)
    if (!_srcState)
    {
      THROW_BASE_RUNTIME_ERROR("Failed to initialize libsamplerate converter: " + std::string(src_strerror(error)));
    }

    // Allocate buffers
    _resampledBuffers[0].resize(_outputFrameCount * 2);
    _resampledBuffers[1].resize(_outputFrameCount * 2);
    _toResampleBuffer.resize(_dataBufferFrameCount * 2);

    // Initialize buffer states
    _isBufferReady[0].store(false);
    _isBufferReady[1].store(false);
    _currentBuffer = 0;
    _currentFrame = 0;
    _resampledFrameCount = 0;
  }

  AudioStream::~AudioStream()
  {
    Stop();

    // Clean up libsamplerate
    if (_srcState)
    {
      src_delete(_srcState);
    }
  }

  std::array<float, 2> AudioStream::GetNextFrame()
  {
    std::array<float, 2> frame = {0, 0};

    // If we are not playing, return silence
    if (!_isPlaying)
    {
      return frame;
    }

    // Check if we need to switch to the next buffer
    if (_currentFrame >= _resampledFrameCount)
    {
      // Try to switch to the next buffer
      int nextBuffer = 1 - _currentBuffer;

      if (_isBufferReady[nextBuffer].load())
      {
        // Mark current buffer as not ready (available for filling)
        _isBufferReady[_currentBuffer].store(false);

        // Switch to next buffer
        _currentBuffer = nextBuffer;
        _currentFrame = 0;

        // Get the frame count for this buffer (should be set by FillBuffers)
        _resampledFrameCount = _bufferedFrameCount[_currentBuffer];

        if (_resampledFrameCount == 0)
        {
          // No data in this buffer either
          return frame;
        }
      }
      else
      {
        // No buffer ready, return silence (underrun)
        return frame;
      }
    }

    // Calculate pan
    float angle = _pan * (PI / 2.f);
    float leftPan = std::cos(angle);
    float rightPan = std::sin(angle);

    // Apply volume and panning
    float left = _resampledBuffers[_currentBuffer][_currentFrame * 2] * _volume * leftPan;
    float right = _resampledBuffers[_currentBuffer][_currentFrame * 2 + 1] * _volume * rightPan;

    frame[0] = left;
    frame[1] = right;

    _currentFrame++;
    return frame;
  }

  void AudioStream::FillBuffers()
  {
    while (_fillBuffers.load())
    {
      // Find a buffer that needs filling
      int bufferToFill = -1;

      for (int i = 0; i < 2; i++)
      {
        if (!_isBufferReady[i].load())
        {
          bufferToFill = i;
          break;
        }
      }

      if (bufferToFill == -1)
      {
        continue;
      }

      // Clear the buffer to fill
      memset(_resampledBuffers[bufferToFill].data(), 0, _resampledBuffers[bufferToFill].size() * sizeof(float));

      // Read frames from decoder
      ma_uint64 readFrames = 0;
      ma_decoder_read_pcm_frames(&_decoder, _toResampleBuffer.data(), _dataBufferFrameCount, &readFrames);

      if (readFrames < _dataBufferFrameCount)
      {
        if (_loop)
        {
          // If looping, seek to front of decoder, and fill in missing frames
          uint64_t remainingFrames = _dataBufferFrameCount - readFrames;
          ma_decoder_seek_to_pcm_frame(&_decoder, 0);
          ma_decoder_read_pcm_frames(&_decoder, _toResampleBuffer.data() + readFrames * 2, remainingFrames, nullptr);
          readFrames = _dataBufferFrameCount;
        }
        else
        {
          // If not, fill with silence
          memset( //
            _toResampleBuffer.data() + readFrames * 2, 0,
            ((_dataBufferFrameCount - readFrames) * 2) * sizeof(int16_t) //
          );

          // Mark last buffer round
          _lastBufferRound = true;
        }
      }

      // Set up SRC_DATA for this conversion
      _srcData.data_in = _toResampleBuffer.data();
      _srcData.data_out = _resampledBuffers[bufferToFill].data();
      _srcData.input_frames = readFrames;
      _srcData.output_frames = _outputFrameCount;
      _srcData.src_ratio = _srcRatio;
      _srcData.end_of_input = _lastBufferRound ? 1 : 0;

      // Perform sample rate conversion
      int error = src_process(_srcState, &_srcData);
      if (error)
      {
        THROW_BASE_RUNTIME_ERROR("Sample rate conversion failed: " + std::string(src_strerror(error)));
      }

      // Store how many frames we actually got
      _bufferedFrameCount[bufferToFill] = _srcData.output_frames_gen;

      // If last buffer round and no frames left
      if (_srcData.output_frames_gen == 0 && _lastBufferRound)
      {
        // Mark as not playing
        _isPlaying = false;
        _lastBufferRound = false;
        break; // Exit the thread loop
      }

      // Mark this buffer as ready
      _isBufferReady[bufferToFill].store(true);
    }
  }

  void AudioStream::SetVolume(float volume)
  {
    _volume = std::clamp(volume, 0.0f, 1.0f);
  }

  void AudioStream::SetPan(float pan)
  {
    _pan = std::clamp(pan, -1.0f, 1.0f);
  }

  void AudioStream::SetLoop(bool loop)
  {
    _loop = loop;
  }

  void AudioStream::Play()
  {
    if (!_fillBuffers.load())
    {
      // Start the buffer filling thread
      _fillBuffers.store(true);
      _bufferThread = std::thread(&AudioStream::FillBuffers, this);
    }

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
    // Reset stream state
    // Stop the buffer thread
    _fillBuffers.store(false);
    if (_bufferThread.joinable())
    {
      _bufferThread.join();
    }

    _isPlaying = false;
    _lastBufferRound = false;
    _currentFrame = 0;
    _currentBuffer = 0;

    // Mark both buffers as not ready so they get refilled
    _isBufferReady[0].store(false);
    _isBufferReady[1].store(false);

    ma_decoder_seek_to_pcm_frame(&_decoder, 0);

    if (_srcState)
    {
      src_reset(_srcState);
    }
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
