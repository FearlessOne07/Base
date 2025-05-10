#include <utility>

#include "base/ui/UIElement.hpp"

namespace Base
{
  void UIElement::SetFont(std::shared_ptr<Font> &font)
  {
    if (font)
    {
      _font = font;
    }
  }

  void UIElement::SetPosition(Vector2 position)
  {
    _position = position;
  }

  UIElement::UIElement(std::shared_ptr<Font> font) : _font(std::move(font))
  {
  }

  void UIElement::Update(float dt)
  {
  }

  void UIElement::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
  }
} // namespace Base
