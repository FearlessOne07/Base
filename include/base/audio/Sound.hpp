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
    std::vector<int16_t> _data;
    uint64_t _frameCount = 0;

  public:
    Sound(std::vector<int16_t> data, uint64_t frameCount);
    std::array<int16_t, 2> GetFrame(uint64_t frame);
    uint64_t GetFrameCount();
  };
} // namespace Base
