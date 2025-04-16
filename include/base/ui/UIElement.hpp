#pragma once
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "raylib/raylib.h"
#include <memory>

namespace Base
{
  class UIElement
  {
    friend class UIManager;

  protected:
    virtual void Render() = 0;
    virtual void Update();
    virtual void OnKeyEvent(const std::shared_ptr<KeyEvent> &event);
    virtual void OnMouseButtonEvent(const std::shared_ptr<MouseButtonEvent> &event);

  public:
    UIElement() = default;
    virtual ~UIElement() = default;
    Vector2 position = {0, 0};
  };
} // namespace Base
