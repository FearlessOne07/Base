#include "base/ui/elements/UIButton.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"

namespace Base
{
  void UIButton::SetColors(Color hoverColor, Color activeColor, Color normalColor, Color textColor)
  {
    _textColor = textColor;
    _hoverColor = hoverColor;
    _activeColor = activeColor;
    _normalColor = normalColor;
  }

  void UIButton::SetText(const std::string &text)
  {
    _text = text;
  }

  Size UIButton::Measure()
  {
    Font font;
    if (_font.Get())
    {
      font = *_font.Get()->GetRaylibFont();
    }
    else
    {
      font = GetFontDefault();
    }

    auto textSize = MeasureTextEx(font, _text.c_str(), _fontSize, 1);
    _desiredSize = {textSize.x, textSize.y};
    _desiredSize.width += _paddingLeft + _paddingRight;
    _desiredSize.height += _paddingTop + _paddingBottom;
    return _desiredSize;
  }

  void UIButton::Arrange(Rectangle finalRect)
  {
    Font font;
    if (_font.Get())
    {
      font = *_font.Get()->GetRaylibFont();
    }
    else
    {
      font = GetFontDefault();
    }

    auto textSize = MeasureTextEx(font, _text.c_str(), _fontSize * _renderTransform.GetFontScale(), 1);
    _layoutRect = finalRect;
    float width = textSize.x;
    float height = textSize.y;
    width += _paddingLeft + _paddingRight;
    height += _paddingTop + _paddingBottom;

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
  }

  void UIButton::SetFontSize(float size)
  {
    _fontSize = size;
  }

  void UIButton::Render(float opacity)
  {

    if (!_isHidden)
    {
      Font font;
      if (_font.Get())
      {
        font = *_font.Get()->GetRaylibFont();
      }
      else
      {
        font = GetFontDefault();
      }

      if (_sprite)
      {
        _sprite.Draw( //
          {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height},
          _renderTransform.GetOpacity() * opacity * 255 //
        );
      }
      else
      {
        DrawRectangleRec( //
          {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height},
          {_color.r, _color.g, _color.b, static_cast<unsigned char>(_renderTransform.GetOpacity() * opacity * 255)} //
        );
      }

      // Measure text size
      float fontSize = _fontSize * _renderTransform.GetFontScale();
      Vector2 textSize = MeasureTextEx(font, _text.c_str(), fontSize, 1);

      // Center text inside padded button area
      Vector2 textPos = {
        _layoutRect.x + (_layoutRect.width - textSize.x) / 2,
        _layoutRect.y + (_layoutRect.height - textSize.y) / 2,
      };

      DrawTextBase( //
        font, _text.c_str(), textPos, fontSize, 1,
        {
          _textColor.r,
          _textColor.g,
          _textColor.b,
          static_cast<unsigned char>(_renderTransform.GetOpacity() * opacity * 255),
        } //
      );
    }
  }

  void UIButton::UpdateElement(float dt, UIContext uiContext)
  {
    _color = _isHovered ? _hoverColor : _normalColor;
  }

  float UIButton::GetFontSize() const
  {
    return _fontSize;
  }

} // namespace Base
