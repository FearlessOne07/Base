#pragma once
#include "base/signals/Event.hpp"
#include <cstdint>

namespace Base
{
  class KeyEvent : public Event
  {
  public:
    enum class Action : uint8_t
    {
      NONE = 0,
      PRESSED,
      HELD,
      RELEASED
    };

    int key = 0;
    Action action = Action::NONE;
    bool isHandled = false;
  };
} // namespace Base
