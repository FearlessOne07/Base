#pragma once

#include <array>
namespace Base
{
  class AudioEffect
  {
  public:
    virtual ~AudioEffect() {};
    virtual std::array<float, 2> Process(const std::array<float, 2> &frame) = 0;
  };
} // namespace Base
