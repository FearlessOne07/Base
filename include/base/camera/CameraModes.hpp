#pragma once
#include <cstdint>
namespace Base
{
  enum struct Camera2DExtMode : uint8_t
  {
    STATIC = 0,
    BASIC_FOLLOW,
    SMOOTH_FOLLOW
  };
} // namespace Base
