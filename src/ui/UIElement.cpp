#include "base/ui/UIElement.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/ui/UILayoutSettings.hpp"
#include "raylib.h"
#include "raymath.h"

namespace Base
{
  UIElement::~UIElement()
  {
  }

  static Rectangle RectangleUnion(Rectangle a, Rectangle b)
  {
    float minX = fminf(a.x, b.x);
    float minY = fminf(a.y, b.y);
    float maxX = fmaxf(a.x + a.width, b.x + b.width);
    float maxY = fmaxf(a.y + a.height, b.y + b.height);

    return {minX, minY, maxX - minX, maxY - minY};
  }

  Rectangle UIElement::GetCombinedHoverRect() const
  {
    Rectangle layoutRect = {_layoutPosition.x, _layoutPosition.y, _currentSize.x, _currentSize.y};
    Rectangle offsetRect = {GetPosition().x, GetPosition().y, _currentSize.x, _currentSize.y};

    return RectangleUnion(layoutRect, offsetRect);
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

  void UIElement::SetVisibilityOff()
  {
    _isHidden = true;
  }

  void UIElement::Hide()
  {
    if (_onHide)
    {
      _onHide();
    }
  }

  bool UIElement::IsVisible() const
  {
    return !_isHidden;
  }

  void UIElement::_update(float dt)
  {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    Vector2 mousePos = rd->mousePosition;

    bool isCurrentlyHovered = CheckCollisionPointRec(mousePos, GetCombinedHoverRect());

    if (isCurrentlyHovered && !_isHovered)
    {
      // Mouse just entered the element
      if (onHover)
      {
        onHover.flex();
      }
    }
    else if (!isCurrentlyHovered && _isHovered)
    {
      // Mouse just exited the elemnt
      if (onHover)
      {
        onHover.relax();
      }
    }
    _isHovered = isCurrentlyHovered;

    Update(dt);
  }
  void UIElement::Update(float dt)
  {
  }

  void UIElement::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
  }
} // namespace Base
