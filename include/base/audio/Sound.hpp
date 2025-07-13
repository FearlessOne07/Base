#pragma once
#include "base/assets/BaseAsset.hpp"
#include <array>
#include <cstdint>
#include <vector>

namespace Base
{

  class Sound : public BaseAsset
  {
  private:
    // Data
    std::vector<float> _data;
    uint64_t _frameCount = 0;
    uint64_t _sampleRate = 0;

  public:
    Sound(std::vector<float> data, uint64_t frameCount, uint64_t sampleRate);
    std::array<float, 2> GetFrame(uint64_t frame) const;
    uint64_t GetFrameCount() const;
    uint64_t GetSampleRate() const;
  };
} // namespace Base
