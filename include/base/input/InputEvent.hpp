
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
      None = 0,
      Pressed,
      Held,
      Released
    };

    Action action = Action::None;
    bool isHandled = false;
  };
} // namespace Base
