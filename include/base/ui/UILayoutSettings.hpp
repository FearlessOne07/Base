#pragma once

#include <cstdint>
namespace Base
{
  enum struct UIVAlignment : uint8_t
  {
    TOP = 0,
    CENTER,
    BOTTOM
  };

  enum struct UIHAlignment : uint8_t
  {
    LEFT = 0,
    CENTER,
    RIGHT,
  };

  enum struct UIContainerSizing
  {
    FILL,
    SHRINK
  };

  struct UILayoutSettings
  {
    UIHAlignment hAlignment = UIHAlignment::LEFT;
    UIVAlignment vAlignment = UIVAlignment::TOP;
    UIContainerSizing vSizing = UIContainerSizing::SHRINK;
    UIContainerSizing hSizing = UIContainerSizing::SHRINK;
  };
} // namespace Base
