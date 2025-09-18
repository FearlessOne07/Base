#include "base/ui/elements/UICanvas.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Draw.hpp"
#include <ranges>

namespace Base
{
  static bool operator==(Color a, Color b)
  {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
  }

  void UICanvas::Arrange(Rectangle finalRect)
  {
    _layoutRect = finalRect;
    float width = _desiredSize.width * _renderTransform.GetScaleX();
    float height = _desiredSize.height * _renderTransform.GetScaleY();

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

    for (auto &child : _childElements)
    {
      Size childSize = child->GetDesiredSize();
      Vector2 childPos = child->GetPosition();

      float childX = _layoutRect.x + childPos.x;
      float childY = _layoutRect.y + childPos.y;

      HAlign childHAlign = child->GetHAlignment();
      VAlign childVAlign = child->GetVAlignment();

      switch (childHAlign)
      {
      case HAlign::Left:
        break;
      case HAlign::Center:
        childX -= childSize.width / 2;
        break;
      case HAlign::Right:
        childX -= childSize.width;
        break;
      case HAlign::Stretch:
        childSize.width = _layoutRect.width;
        break;
      }

      switch (childVAlign)
      {
      case VAlign::Top:
        break;
      case VAlign::Center:
        childY -= childSize.height / 2;
        break;
      case VAlign::Bottom:
        childY -= childSize.height;
        break;
      case VAlign::Stretch:
        childSize.height = _layoutRect.height;
        break;
      }

      child->Arrange({childX, childY, childSize.width, childSize.height});
    }
  }

  void UICanvas::Render(float opacity)
  {
    if (!_isHidden)
    {
      // Render canvas background
      if (_sprite)
      {
        _sprite.Draw(_layoutRect, _renderTransform.GetOpacity() * opacity * 255);
      }
      else if (_fillColor.a > 0) // Only draw if not transparent
      {
        float alpha = _renderTransform.GetOpacity();
        DrawRectangleBase( //
          _layoutRect, {0, 0}, 0,
          {
            _fillColor.r,
            _fillColor.g,
            _fillColor.b,
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

  void UICanvas::SetFillColor(Color color)
  {
    _fillColor = color;
  }
} // namespace Base
