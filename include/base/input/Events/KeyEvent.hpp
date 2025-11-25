#pragma once
#include "base/input/InputEvent.hpp"
#include "base/input/Keys.hpp"

namespace Base
{
  class KeyEvent : public InputEvent
  {
  public:
    Base::Key Key = Key::Null;
  };
} // namespace Base
