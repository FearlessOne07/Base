#include "base/util/Easings.hpp"

namespace Base::Easings
{
  float EaseInCubic(float t)
  {
    return t * t * t;
  }

  float EaseOutCubic(float t)
  {
    t -= 1;
    return (t * t * t) + 1;
  }

  float EaseInOutCubic(float t)
  {
    return t < 0.5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
  }
} // namespace Base::Easings
