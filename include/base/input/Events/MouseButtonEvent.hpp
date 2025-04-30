#pragma once
#include "base/input/InputEvent.hpp"
namespace Base
{
  class MouseButtonEvent : public InputEvent
  {
  public:
    int button = 0;
  };
} // namespace Base
