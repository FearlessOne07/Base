#include "base/ui/UIElement.hpp"

namespace Base
{
  void UIElement::SetFont(const std::shared_ptr<Font> &font)
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

  Vector2 UIElement::GetPosition() const
  {
    return _position;
  }

  Vector2 UIElement::GetSize() const
  {
    return _size;
  }
  void UIElement::Update(float dt)
  {
  }

  void UIElement::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
  }
} // namespace Base
