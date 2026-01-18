#include "base/ui/elements/UIFlexContainer.hpp"
#include "base/ui/UIElement.hpp"
#include "internal/rendering/Renderer.hpp"
#include <ranges>

namespace Base
{

  static bool operator==(Color a, Color b)
  {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
  }

  Size UIFlexContainer::Measure()
  {

    Size total = {0, 0};
    for (auto it = _childElements.begin(); it != _childElements.end(); it++)
    {
      auto &child = *it;
      Size csize = child->Measure();
      if (_orientation == Orientation::Vertical)
      {

        if (_widthSizeMode == SizeMode::Auto)
        {
          total.width = std::max(total.width, csize.width);
        }

        if (_heightSizeMode == SizeMode::Auto)
        {
          total.height += csize.height;
        }
      }
      else
      {
        if (_heightSizeMode == SizeMode::Auto)
        {
          total.height = std::max(total.height, csize.height);
        }

        if (_widthSizeMode == SizeMode::Auto)
        {
          total.width += csize.width;
        }
      }
    }
    total.height += _paddingBottom + _paddingTop;
    total.width += _paddingLeft + _paddingRight;

    if (_widthSizeMode == SizeMode::Auto)
    {
      _desiredSize.width = total.width;
    }
    if (_heightSizeMode == SizeMode::Auto)
    {
      _desiredSize.height = total.height;
    }

    return _desiredSize;
  }

  void UIFlexContainer::Arrange(Rectangle finalRect)
  {
    _layoutRect = finalRect;
    float width = _desiredSize.width * _renderTransform.GetScaleX();
    float height = _desiredSize.height * _renderTransform.GetScaleY();

    if (_orientation == Orientation::Vertical)
    {
      _verticalAlignment = VAlign::Stretch;
    }
    else
    {
      _horizontalAlignment = HAlign::Stretch;
    }

    Vector2 finalPos(0);
    Vector2 finalSize = {width, height};

    // Horizontal alignment
    switch (_horizontalAlignment)
    {
    case HAlign::Left:
      break;
    case HAlign::Center:
      finalPos.x += (finalRect.GetSize().x - width) / 2;
      break;
    case HAlign::Right:
      finalPos.x += finalRect.GetSize().x - width;
      break;
    case HAlign::Stretch:
      finalSize.x = finalRect.GetSize().x;
      break;
    }

    // Vertical alignment
    switch (_verticalAlignment)
    {
    case VAlign::Top:
      break;
    case VAlign::Center:
      finalPos.y += (finalRect.GetSize().y - height) / 2;
      break;
    case VAlign::Bottom:
      finalPos.y += finalRect.GetSize().y - height;
      break;
    case VAlign::Stretch:
      finalSize.y = finalRect.GetSize().y;
      break;
    }
    _layoutRect = {finalPos + Vector2{_renderTransform.GetOffsetx(), _renderTransform.GetOffsetY()}, finalSize};

    float rectHeight = (_layoutRect.GetSize().y - (_paddingTop + _paddingBottom)) / _childElements.size();
    float rectWidth = (_layoutRect.GetSize().x - (_paddingLeft + _paddingRight)) / _childElements.size();
    float offset = (_orientation == Orientation::Vertical) ? _layoutRect.GetPosition().y + _paddingTop
                                                           : _layoutRect.GetPosition().x + _paddingLeft;
    for (auto &child : _childElements)
    {
      Size csize = child->GetDesiredSize();
      if (_orientation == Orientation::Vertical)
      {
        child->Arrange({
            {_layoutRect.GetPosition().x + _paddingLeft, offset},
            {_layoutRect.GetSize().x - (_paddingLeft + _paddingRight), rectHeight},
        } //
        );
        offset += rectHeight;
      }
      else
      {
        child->Arrange({
            {offset, _layoutRect.GetPosition().y + _paddingTop},
            {rectWidth, _layoutRect.GetSize().y - (_paddingTop + _paddingBottom)},
        } //
        );
        offset += rectWidth;
      }
    }
  }

  void UIFlexContainer::Render(float opacity)
  {
    if (!_isHidden)
    {
      if (_sprite)
      {
        _sprite.Draw({_layoutRect.GetPosition(), _layoutRect.GetSize()}, _renderTransform.GetOpacity() * opacity * 255);
      }
      else
      {
        float alpha = 0;
        if (_backgroundColor == Color{0, 0, 0, 0})
        {
          alpha = 0;
        }
        else
        {
          alpha = _renderTransform.GetOpacity();
        }
        Renderer::DrawQuad(_layoutRect, _layoutRect.GetPosition(), _backgroundColor);
      }

      auto elements = std::ranges::reverse_view(_childElements);
      for (auto &element : elements)
      {
        if (element->IsVisible())
        {
          element->Render(_renderTransform.GetOpacity() * opacity);
        }
      }
    }
  }

  void UIFlexContainer::SetBackgroundColor(Color color)
  {
    _backgroundColor = color;
  }

  void UIFlexContainer::SetOrientation(Orientation orientation)
  {
    _orientation = orientation;
  }
} // namespace Base
