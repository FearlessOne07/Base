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
      PRESSED,
      DOWN
    };
  };
} // namespace Base
