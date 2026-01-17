#include "base/ui/elements/UIButton.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Type.hpp"
#include "internal/rendering/Renderer.hpp"

namespace Base
{
  static bool operator==(Color a, Color b)
  {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
  }

  void UIButton::SetTextColor(Color textColor)
  {
    _textColor = textColor;
  }

  void UIButton::SetBackgroundColor(Color backgroundColor)
  {
    _backgroundColor = backgroundColor;
  }

  void UIButton::SetText(const std::string &text)
  {
    _text = text;
  }

  Size UIButton::Measure()
  {
    auto textSize = Font::MeasureText(_font.Get(), _text, _fontSize);
    _desiredSize = {textSize.x, textSize.y};
    _desiredSize.width += _paddingLeft + _paddingRight;
    _desiredSize.height += _paddingTop + _paddingBottom;
    return _desiredSize;
  }

  void UIButton::Arrange(Rectangle finalRect)
  {
    auto textSize = Font::MeasureText(_font.Get(), _text, _fontSize * _renderTransform.GetFontScale());
    _layoutRect = finalRect;
    float width = textSize.x;
    float height = textSize.y;
    width += _paddingLeft + _paddingRight;
    height += _paddingTop + _paddingBottom;

    Vector2 finalPos;
    Vector2 finalSize;

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
    _layoutRect = {finalPos, finalSize};
  }

  void UIButton::SetFontSize(float size)
  {
    _fontSize = size;
  }

  void UIButton::Render(float opacity)
  {

    if (!_isHidden)
    {
      if (_sprite)
      {
        // Nine Patch
        // Renderer::DrawSprite(_sprite, _layoutRect.GetPosition(), _layoutRect.GetSize());
        // _sprite.Draw( //
        //   {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height},
        //   _renderTransform.GetOpacity() * opacity * 255 //
        // );
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

        Renderer::DrawQuad({_layoutRect.GetSize()}, _layoutRect.GetPosition(), _backgroundColor);
      }

      // Measure text size
      float fontSize = _fontSize * _renderTransform.GetFontScale();
      Vector2 textSize = Font::MeasureText(_font.Get(), _text, fontSize);

      // Center text inside padded button area
      Vector2 textPos = {
        _layoutRect.GetPosition().x + (_layoutRect.GetSize().x - textSize.x) / 2,
        _layoutRect.GetPosition().y + (_layoutRect.GetSize().y - textSize.y) / 2,
      };

      Renderer::DrawText(_text, textPos, _textColor, fontSize, _font.Get());
    }
  }

  float UIButton::GetFontSize() const
  {
    return _fontSize;
  }

} // namespace Base
