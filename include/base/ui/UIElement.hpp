#pragma once

#include "raylib/raylib.h"
namespace Base
{
  class UIElement
  {
    friend class UIManager;

  protected:
    virtual void Render() = 0;
    virtual void Update();
    virtual void HandleInput();

  public:
    virtual ~UIElement() = default;
    UIElement() = default;
    Vector2 position = {0, 0};
  };
} // namespace Base
