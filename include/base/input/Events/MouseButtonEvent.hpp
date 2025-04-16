#pragma once
#include "base/Event.hpp"
#include <cstdint>

namespace Base
{
  class MouseButtonEvent : public Event
  {
  public:
    enum struct Action : std::uint8_t
    {
      NONE,
      PRESSED,
      HELD,
      RELEASED
    };

    int button = 0;
    Action action = Action::NONE;
    bool isHandled = false;
  };
} // namespace Base
