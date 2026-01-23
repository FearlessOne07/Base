#pragma once
#include <cstdint>
namespace Base
{
  enum struct CameraMode : uint8_t
  {
    Static = 0,
    BasicFollow,
    SmoothFollow
  };
} // namespace Base
