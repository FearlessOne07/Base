#include "base/audio/Sound.hpp"
#include <cstdint>
#include <rubberband/RubberBandStretcher.h>

namespace Base
{
  Sound::Sound(std::vector<float> data, uint64_t frameCount, uint64_t sampleRate)
    : _data(data), _frameCount(frameCount), _sampleRate(sampleRate)
  {
  }

  std::array<float, 2> Sound::GetFrame(uint64_t frameIndex) const
  {
    std::array<float, 2> frame;

    // If frame is in range
    if (frameIndex < _frameCount)
    {
      // Get frame at index
      frame[0] = _data[frameIndex * 2];
      frame[1] = _data[frameIndex * 2 + 1];
    }
    else
    {
      // Else return silence
      frame[0] = 0;
      frame[1] = 0;
    }

    return frame;
  }

  uint64_t Sound::GetFrameCount() const
  {
    return _frameCount;
  }

  uint64_t Sound::GetSampleRate() const
  {
    return _sampleRate;
  }
} // namespace Base
