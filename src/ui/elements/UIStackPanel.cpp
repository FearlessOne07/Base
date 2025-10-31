#include "base/ui/elements/UIStackPanel.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"
#include <algorithm>
#include <iterator>
#include <ranges>

namespace Base
{
  static bool operator==(Color a, Color b)
  {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
  }

  Size UIStackPanel::Measure()
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
          if (std::distance(it, _childElements.end()) > 1)
          {
            total.height += _gap;
          }
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
          if (std::distance(it, _childElements.end()) > 1)
          {
            total.width += _gap;
          }
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
  };

  void UIStackPanel::Arrange(Rectangle finalRect)
  {
    _layoutRect = finalRect;
    float width =  _desiredSize.width * _renderTransform.GetScaleX();
    float height = _desiredSize.height * _renderTransform.GetScaleY();

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

    _layoutRect.x += _renderTransform.GetOffsetx();
    _layoutRect.y += _renderTransform.GetOffsetY();

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
          csize.height,
        } //
        );
        offset += csize.height + _gap;
      }
      else
      {
        child->Arrange({
          offset,
          _layoutRect.y + _paddingTop,
          csize.width,
          _layoutRect.height - (_paddingTop + _paddingBottom),
        } //
        );
        offset += csize.width + _gap;
      }
    }
  }

  void UIStackPanel::SetGap(float gap)
  {
    _gap = gap;
  }

  void UIStackPanel::SetOrientation(Orientation orientation)
  {
    _orientation = orientation;
  }

  void UIStackPanel::SetBackgroundColor(Color color)
  {
    _backgroundColor = color;
  }

  void UIStackPanel::Render(float opacity)
  {
    if (!_isHidden)
    {
      if (_sprite)
      {
        _sprite.Draw(_layoutRect, _renderTransform.GetOpacity() * opacity * 255);
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
        element->Render(_renderTransform.GetOpacity() * opacity);
      }
    }
  }

  void UIStackPanel::Hide()
  {
    if (onHide)
    {
      onHide();
    }

    for (auto &element : _childElements)
    {
      element->Hide();
    }
  }

  void UIStackPanel::Show()
  {
    _isHidden = false;
    if (onShow)
    {
      onShow();
    }

    for (auto &element : _childElements)
    {
      element->Show();
    }
  }
} // namespace Base
