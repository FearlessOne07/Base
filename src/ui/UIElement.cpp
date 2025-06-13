#include "base/ui/UIElement.hpp"

namespace Base
{
  void UIElement::SetFont(const AssetHandle<BaseFont> &font)
  {
    if (font.Get())
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
    _setPosition = position;
  }

  Vector2 UIElement::GetPosition() const
  {
    return _position;
  }

  Vector2 UIElement::GetSize() const
  {
    return _size;
  }

  void UIElement::_update(float dt)
  {
    UpdatePosition();
    Update(dt);
  }

  void UIElement::UpdatePosition()
  {
    switch (_anchorPoint)
    {
    case AnchorPoint::TOP_LEFT: {
      _position = _setPosition;
      break;
    }
    case AnchorPoint::TOP_RIGHT: {
      _position.x = _setPosition.x - _size.x;
      _position.y = _setPosition.y;
      break;
    }
    case AnchorPoint::CENTER: {
      _position.x = _setPosition.x - _size.x / 2;
      _position.y = _setPosition.y - _size.y / 2;
      break;
    }
    case AnchorPoint::BOTTOM_LEFT: {
      _position.x = _setPosition.x;
      _position.y = _setPosition.y - _size.y;
      break;
    }
    case AnchorPoint::BOTTOM_RIGHT: {
      _position.x = _setPosition.x - _size.x;
      _position.y = _setPosition.y - _size.y;
      break;
    }
    }
  }

  void UIElement::Update(float dt)
  {
  }

  void UIElement::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
  }
} // namespace Base
