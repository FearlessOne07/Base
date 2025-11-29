#pragma once

#include <cstdint>
namespace Base::Easings
{
  enum class Type : uint8_t
  {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut
  };
  float EaseLinear(float t);
  float EaseInCubic(float t);
  float EaseOutCubic(float t);
  float EaseInOutCubic(float t);
} // namespace Base::Easings
