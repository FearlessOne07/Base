#include "base/ui/UIElement.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/input/MouseButtons.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/ui/UIConext.hpp"
#include "base/util/Type.hpp"
#include "internal/utils/Collisions.hpp"
#include <algorithm>
#include <cmath>

namespace Base
{
  static Rectangle RectangleUnion(Rectangle a, Rectangle b)
  {
    float minX = fminf(a.GetPosition().x, b.GetPosition().x);
    float minY = fminf(a.GetPosition().y, b.GetPosition().y);
    float maxX = fmaxf(a.GetPosition().x + a.GetSize().x, b.GetPosition().x + b.GetSize().x);
    float maxY = fmaxf(a.GetPosition().y + a.GetSize().y, b.GetPosition().y + b.GetSize().y);

    return {{minX, minY}, {maxX - minX, maxY - minY}};
  }

  Rectangle UIElement::GetCombinedHoverRect() const
  {
    Rectangle layoutRect = {
      {_layoutRect.GetPosition().x - _renderTransform.GetOffsetx(),
       _layoutRect.GetPosition().y - _renderTransform.GetOffsetY()},
      {
        _layoutRect.GetSize().x,
        _layoutRect.GetSize().y,
      },
    };
    return RectangleUnion(layoutRect, _layoutRect);
  }

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

  void UIElement::SetFont(const AssetHandle<Font> &font)
  {
    if (font.Get())
    {
      _font = font;
    }
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

  void UIElement::SetWidth(float width)
  {
    _desiredSize.width = width;
  }

  void UIElement::SetHeight(float height)
  {
    _desiredSize.height = height;
  }

  void UIElement::SetWidthSizeMode(SizeMode mode)
  {
    _widthSizeMode = mode;
  }

  void UIElement::SetHeightSizeMode(SizeMode mode)
  {
    _heightSizeMode = mode;
  }

  void UIElement::SetSprite(const NinePatchSprite &sprite)
  {
    _sprite = sprite;
  }

  void UIElement::SetSizeMode(SizeMode mode)
  {
    _widthSizeMode = mode;
    _heightSizeMode = mode;
  }

  void UIElement::SetSize(Size size)
  {
    _desiredSize = size;
  }

  void UIElement::SetPosition(Vector2 position)
  {
    _desiredPosition = position;
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
        if (                                                                                                         //
          mouseEvent->action == Base::InputEvent::Action::Held && mouseEvent->Button == MouseKey::Left && _isHovered //
        )
        {
          _isActive = true;
          event->isHandled = true;
        }
        else if ( //
          mouseEvent->action == Base::InputEvent::Action::Released && mouseEvent->Button == MouseKey::Left &&
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

  void UIElement::Update(float dt, UIContext uiContext)
  {
    Vector2 mousePos = uiContext.MousePosition;

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
    UpdateElement(dt, uiContext);
  }

  void UIElement::OnElementInputEvent(std::shared_ptr<InputEvent> &event)
  {
    for (auto &element : _childElements)
    {
      if (element->IsVisible())
      {
        element->OnInputEvent(event);
      }
      if (event->isHandled)
      {
        break;
      }
    }
  }

  void UIElement::UpdateElement(float dt, UIContext uiContext)
  {
    for (auto &child : _childElements)
    {
      if (child->IsVisible())
      {
        child->Update(dt, uiContext);
      }
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
    float width = _desiredSize.width * _renderTransform.GetScaleX();
    float height = _desiredSize.height * _renderTransform.GetScaleY();

    Vector2 pos;

    // Horizontal alignment
    switch (_horizontalAlignment)
    {
    case HAlign::Left:
      break;
    case HAlign::Center:
      pos.x += (finalRect.GetSize().x - width) / 2;
      break;
    case HAlign::Right:
      pos.x += finalRect.GetSize().x - width;
      break;
    case HAlign::Stretch:
      width = finalRect.GetSize().x;
      break;
    }

    // Vertical alignment
    switch (_verticalAlignment)
    {
    case VAlign::Top:
      break;
    case VAlign::Center:
      pos.y += (finalRect.GetSize().y - height) / 2;
      break;
    case VAlign::Bottom:
      pos.y += finalRect.GetSize().y - height;
      break;
    case VAlign::Stretch:
      height = finalRect.GetSize().y;
      break;
    }

    _layoutRect = {{width, height}, pos};

    for (auto &child : _childElements)
    {
      child->Arrange(_layoutRect);
    }
  }

  RenderTransform &UIElement::GetRenderTransform()
  {
    return _renderTransform;
  }

  Size UIElement::GetDesiredSize() const
  {
    return _desiredSize;
  }

  Vector2 UIElement::GetPosition() const
  {
    return _desiredPosition;
  }

  VAlign UIElement::GetVAlignment() const
  {
    return _verticalAlignment;
  }

  HAlign UIElement::GetHAlignment() const
  {
    return _horizontalAlignment;
  }

  bool UIElement::HasChild(const std::string &name) const
  {
    std::string lowerid = Base::Strings::ToLower(name);
    if (auto it = std::ranges::find(_childElementIds, lowerid); it == _childElementIds.end())
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  void UIElement::RemoveChild(const std::string &id)
  {
    std::string lowerid = Base::Strings::ToLower(id);
    if (auto it = std::ranges::find(_childElementIds, lowerid); it != _childElementIds.end())
    {
      int index = static_cast<int>(std::distance(_childElementIds.begin(), it));
      _childElementIds.erase(it);
      _childElements.erase(_childElements.begin() + index);
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Element " + id + " isn't registerd in layer");
    }
  }
} // namespace Base
