#pragma once

#include "base/input/InputEvent.hpp"
#include <memory>
namespace Base
{
  class InputListener
  {
  public:
    virtual ~InputListener() = default;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event) = 0;
  };
} // namespace Base
