#pragma once
#include <cstdint>
namespace Base
{
  enum class MouseKey : std::uint8_t
  {
    Null = 0,
    Left = 1,
    Right = 2,
    Middle = 3,
    Side = 4,
    Extra = 5,
    Forward = 6,
    Back = 7
  };
} // namespace Base
