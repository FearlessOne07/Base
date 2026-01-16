#include "base/ui/elements/UICanvas.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Type.hpp"
#include "internal/rendering/Renderer.hpp"
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

    Vector2 finalPos;
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

    for (auto &child : _childElements)
    {
      Size childSize = child->GetDesiredSize();
      Vector2 childPos = child->GetPosition();

      float childX = _layoutRect.GetPosition().x + childPos.x;
      float childY = _layoutRect.GetPosition().y + childPos.y;

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
        childSize.width = _layoutRect.GetSize().x;
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
        childSize.height = _layoutRect.GetSize().y;
        break;
      }

      child->Arrange({{childX, childY}, {childSize.width, childSize.height}});
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
        Renderer::DrawQuad(_layoutRect, _layoutRect.GetPosition(), _fillColor);
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
