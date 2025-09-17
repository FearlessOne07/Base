#include "base/ui/UIElement.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "raylib.h"
#include <algorithm>

namespace Base
{
  UIElement::~UIElement()
  {
  }

  void UIElement::SetHAlignment(HAlign hAlign)
  {
    _horizontalAlignment = hAlign;
  }

  void UIElement::SetVAlignment(VAlign vAlign)
  {
    _verticalAlignment = vAlign;
  }

  void UIElement::SetFont(const AssetHandle<BaseFont> &font)
  {
    if (font.Get())
    {
      _font = font;
    }
  }

  void UIElement::SetAlpha(float alpha)
  {
    _alpha = alpha;
    _alpha = std::clamp<float>(_alpha, 0, 1);
  }

  void UIElement::SetPadding(float padding)
  {
    _paddingTop = padding;
    _paddingBottom = padding;
    _paddingLeft = padding;
    _paddingRight = padding;
  }

  void UIElement::SetPadding(float paddingX, float paddingY)
  {
    _paddingLeft = paddingX;
    _paddingRight = paddingX;
    _paddingTop = paddingY;
    _paddingBottom = paddingY;
  }

  void UIElement::SetPadding(float paddingLeft, float paddingRight, float paddingTop, float paddingBottom)
  {
    _paddingTop = paddingTop;
    _paddingBottom = paddingBottom;
    _paddingLeft = paddingLeft;
    _paddingRight = paddingRight;
  }

  void UIElement::SetParentAlpha(float alpha)
  {
    _parentAlpha = alpha;
    _parentAlpha = std::clamp<float>(_parentAlpha, 0, 1);
  }

  void UIElement::SetSize(Size size)
  {
    _desiredSize = size;
  }

  void UIElement::SetSprite(const NinePatchSprite &sprite)
  {
    _sprite = sprite;
  }

  void UIElement::SetRenderTransform(const RenderTransform &transform)
  {
    _renderTransform = transform;
  }

  void UIElement::Show()
  {
    _isHidden = false;
    if (onShow)
    {
      onShow();
    }
  }

  void UIElement::SetVisibilityOff()
  {
    _isHidden = true;
  }

  void UIElement::Hide()
  {
    if (onHide)
    {
      onHide();
    }
  }

  bool UIElement::IsVisible() const
  {
    return !_isHidden;
  }

  void UIElement::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    if (auto mouseEvent = std::dynamic_pointer_cast<Base::MouseButtonEvent>(event))
    {
      if (onClick)
      {
        if ( //
          mouseEvent->action == Base::InputEvent::Action::HELD && mouseEvent->button == MOUSE_BUTTON_LEFT &&
          _isHovered //
        )
        {
          _isActive = true;
          event->isHandled = true;
        }
        else if ( //
          mouseEvent->action == Base::InputEvent::Action::RELEASED && mouseEvent->button == MOUSE_BUTTON_LEFT &&
          _isActive //
        )
        {
          onClick();
          _isActive = false;
          event->isHandled = true;
        }
        else
        {
          _isActive = false;
        }
      }
    }

    OnElementInputEvent(event);
  }

  void UIElement::Update(float dt)
  {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    Vector2 mousePos = rd->mousePosition;

    bool isCurrentlyHovered = CheckCollisionPointRec(mousePos, _layoutRect);

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
    UpdateElement(dt);
  }

  void UIElement::OnElementInputEvent(std::shared_ptr<InputEvent> &event)
  {
  }

  void UIElement::UpdateElement(float dt)
  {
    for (auto &child : _childElements)
    {
      child->Update(dt);
    }
  }

  Size UIElement::Measure()
  {
    for (auto &child : _childElements)
    {
      child->Measure();
    }

    return _desiredSize;
  }

  void UIElement::Arrange(Rectangle finalRect)
  {
    _layoutRect = finalRect;
    float width = _desiredSize.width * _renderTransform.scaleX;
    float height = _desiredSize.height * _renderTransform.scaleY;

    // Horizontal alignment
    switch (_horizontalAlignment)
    {
    case HAlign::Left:
      break;
    case HAlign::Center:
      _layoutRect.x += (finalRect.width - width) / 2;
      break;
    case HAlign::Right:
      _layoutRect.x += finalRect.width - width;
      break;
    case HAlign::Stretch:
      width = finalRect.width;
      break;
    }

    // Vertical alignment
    switch (_verticalAlignment)
    {
    case VAlign::Top:
      break;
    case VAlign::Center:
      _layoutRect.y += (finalRect.height - height) / 2;
      break;
    case VAlign::Bottom:
      _layoutRect.y += finalRect.height - height;
      break;
    case VAlign::Stretch:
      height = finalRect.height;
      break;
    }

    _layoutRect.width = width;
    _layoutRect.height = height;

    for (auto &child : _childElements)
    {
      child->Arrange(_layoutRect);
    }
  }

  const RenderTransform &UIElement::GetRenderTransform() const
  {
    return _renderTransform;
  }

  Size UIElement::GetDesiredSize() const
  {
    return _desiredSize;
  }
} // namespace Base
