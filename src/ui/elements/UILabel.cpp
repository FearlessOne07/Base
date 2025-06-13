#include "base/ui/elements/UILabel.hpp"
#include "raylib.h"
#include <string>

namespace Base
{
  void UILabel::SetText(const std::string &text)
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
    _text = text;
    _size = MeasureTextEx(font, text.c_str(), _fontSize, 1);
  }

  void UILabel::SetFontSize(float size)
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
    _fontSize = size;
    _size = MeasureTextEx(font, _text.c_str(), _fontSize, 1);
  }

  void UILabel::SetTextColor(Color color)
  {
    _textColor = color;
  }

  void UILabel::Render()
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

    DrawTextEx(font, _text.c_str(), _position, _fontSize, 1, _textColor);
  }
} // namespace Base
