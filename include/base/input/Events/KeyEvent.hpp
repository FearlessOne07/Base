#pragma once
#include "base/input/InputEvent.hpp"

namespace Base
{
  class KeyEvent : public InputEvent
  {
  public:
    int key = 0;
  };
} // namespace Base
