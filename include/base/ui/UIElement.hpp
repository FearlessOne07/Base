#pragma once

#include "base/input/InputEvent.hpp"
#include <memory>
#include <raylib.h>

namespace Base
{
  class UIElement
  {
  protected:
    std::shared_ptr<Font> _font = nullptr;
    Vector2 _position = {0, 0};
    Vector2 _size = {0, 0};

  public:
    void SetPosition(Vector2 position);
    void GetPosition() const;
    void SetFont(std::shared_ptr<Font> &);
    virtual void Render() = 0;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event);
    virtual void Update(float dt);
  };
} // namespace Base
