#include "base/ui/elements/UIButton.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"

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

  float UIButton::GetFontSize() const
  {
    return _fontSize;
  }

} // namespace Base
