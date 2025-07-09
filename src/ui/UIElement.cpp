#include "base/ui/UIElement.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/sprites/Sprite.hpp"
#include "base/ui/UILayoutSettings.hpp"
#include "raylib.h"
#include "raymath.h"

namespace Base
{
  UIElement::~UIElement()
  {
  }

  void UIElement::SetFont(const AssetHandle<BaseFont> &font)
  {
    if (font.Get())
    {
      _font = font;
    }
  }

  void UIElement::SetPosition(Vector2 position)
  {
    _basePosition = position;
  }

  void UIElement::SetPositionalOffset(Vector2 offset)
  {
    _positionalOffset = offset;
  }

  void UIElement::SetSize(Vector2 size, bool base)
  {
    if (base)
    {
      _baseSize = size;
    }
    _currentSize = size;
  }

  void UIElement::SetSprite(const NinePatchSprite &sprite)
  {
    _sprite = sprite;
  }

  void UIElement::SetContainterSizeMode(UIElement::ContainerSizeMode sizeMode)
  {
    _containerSizeMode = sizeMode;
  }

  void UIElement::SetElementSizeMode(UIElement::ElementSizeMode sizeMode)
  {
    _elementSizeMode = sizeMode;
  }

  void UIElement::SetLayoutSettings(const UILayoutSettings &settings)
  {
    _layoutSettings = settings;
  }

  Vector2 UIElement::GetPosition() const
  {
    return _layoutPosition + _positionalOffset;
  }

  Vector2 UIElement::GetPositionalOffset() const
  {
    return _positionalOffset;
  }

  Vector2 UIElement::GetSize() const
  {
    return _currentSize;
  }

  UIElement::ContainerSizeMode UIElement::GetContainerSizeMode() const
  {
    return _containerSizeMode;
  }

  UIElement::ElementSizeMode UIElement::GetElementSizeMode() const
  {
    return _elementSizeMode;
  }

  Vector2 UIElement::GetBaseSize() const
  {
    return _baseSize;
  }

  const UILayoutSettings &UIElement::GetLayoutSettings() const
  {
    return _layoutSettings;
  }

  void UIElement::Show()
  {
    _isHidden = false;
    if (_onShow)
    {
      _onShow();
    }
  }

  void UIElement::Hide()
  {
    if (_onHide)
    {
      _onHide();
    }
    _isHidden = true;
  }

  bool UIElement::IsVisible() const
  {
    return !_isHidden;
  }

  void UIElement::Update(float dt)
  {
  }

  void UIElement::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
  }
} // namespace Base
