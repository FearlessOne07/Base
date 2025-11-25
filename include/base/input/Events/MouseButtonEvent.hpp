#pragma once
#include "base/input/InputEvent.hpp"
#include "base/input/MouseButtons.hpp"
namespace Base
{
  class MouseButtonEvent : public InputEvent
  {
  public:
    MouseKey Button = MouseKey::Null;
  };
} // namespace Base
