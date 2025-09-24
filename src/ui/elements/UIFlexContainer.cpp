#include "base/ui/elements/UIFlexContainer.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Draw.hpp"
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
    _layoutRect.width = std::min(width, finalRect.width);
    _layoutRect.height = std::min(height, finalRect.height);

    _layoutRect.x += _renderTransform.GetScaleX();
    _layoutRect.y += _renderTransform.GetOffsetY();

    float rectHeight = (_layoutRect.height - (_paddingTop + _paddingBottom)) / _childElements.size();
    float rectWidth = (_layoutRect.width - (_paddingLeft + _paddingRight)) / _childElements.size();
    float offset = (_orientation == Orientation::Vertical) ? _layoutRect.y + _paddingTop : _layoutRect.x + _paddingLeft;
    for (auto &child : _childElements)
    {
      Size csize = child->GetDesiredSize();
      if (_orientation == Orientation::Vertical)
      {
        child->Arrange({
          _layoutRect.x + _paddingLeft,
          offset,
          _layoutRect.width - (_paddingLeft + _paddingRight),
          rectHeight,
        } //
        );
        offset += rectHeight;
      }
      else
      {
        child->Arrange({
          offset,
          _layoutRect.y + _paddingTop,
          rectWidth,
          _layoutRect.height - (_paddingTop + _paddingBottom),
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
        _sprite.Draw({_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height},
                     _renderTransform.GetOpacity() * opacity * 255);
      }
      else
      {
        float alpha = 0;
        if (_backgroundColor == BLANK)
        {
          alpha = 0;
        }
        else
        {
          alpha = _renderTransform.GetOpacity();
        }
        DrawRectangleBase( //
          {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height}, {0, 0}, 0,
          {
            _backgroundColor.r,
            _backgroundColor.g,
            _backgroundColor.b,
            static_cast<unsigned char>(alpha * opacity * 255),
          } //
        );
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
