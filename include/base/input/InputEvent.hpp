
#pragma once
#include <cstdint>

namespace Base
{
  class InputEvent
  {
  public:
    virtual ~InputEvent() = default;

    enum class Action : uint8_t
    {
      NONE = 0,
      PRESSED,
      HELD,
      RELEASED
    };

    Action action = Action::NONE;
    bool isHandled = false;
  };
} // namespace Base
