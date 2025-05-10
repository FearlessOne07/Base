#pragma once

#include "base/input/InputEvent.hpp"
#include <memory>
#include <raylib.h>

namespace Base
{
  class UIElement
  {
  private:
    Rectangle _rect;

  public:
    virtual void Render() = 0;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event);
    virtual void Update(float dt);
  };
} // namespace Base
