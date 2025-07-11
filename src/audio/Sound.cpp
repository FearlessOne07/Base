#include "base/audio/Sound.hpp"
#include <cstdint>

namespace Base
{
  Sound::Sound(std::vector<float> data, uint64_t frameCount) : _data(data), _frameCount(frameCount)
  {
  }

  std::array<float, 2> Sound::GetFrame(uint64_t frameIndex)
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

  uint64_t Sound::GetFrameCount()
  {
    return _frameCount;
  }
} // namespace Base
