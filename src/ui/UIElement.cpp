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
  void UIElement::SetAnchorPoint(AnchorPoint anchorPoint)
  {
    _anchorPoint = anchorPoint;
  }

  void UIElement::SetPosition(Vector2 position)
  {
    switch (_anchorPoint)
    {
    case AnchorPoint::TOP_LEFT: {
      _position = position;
      break;
    }
    case AnchorPoint::TOP_RIGHT: {
      _position.x = position.x - _size.x;
      _position.y = position.y;
      break;
    }
    case AnchorPoint::CENTER: {
      _position.x = position.x - _size.x / 2;
      _position.y = position.y - _size.y / 2;
      break;
    }
    case AnchorPoint::BOTTOM_LEFT: {
      _position.x = position.x;
      _position.y = position.y - _size.y;
      break;
    }
    case AnchorPoint::BOTTOM_RIGHT: {
      _position.x = position.x - _size.x;
      _position.y = position.y - _size.y;
      break;
    }
    }
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
