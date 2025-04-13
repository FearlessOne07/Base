#pragma once

namespace Base
{
  struct MouseButtonState
  {
    bool active = false;
    bool handled = false;
  };
  struct MouseState
  {
    MouseButtonState mouseLeftPressed = {.active = false, .handled = false};
    MouseButtonState mouseRightPressed = {.active = false, .handled = false};

    MouseButtonState mouseLeftDown = {.active = false, .handled = false};
    MouseButtonState mouseRightDown = {.active = false, .handled = false};
  };
} // namespace Base
