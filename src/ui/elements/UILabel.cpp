#include "base/ui/elements/UILabel.hpp"
#include "raylib.h"
#include <string>

namespace Base
{
  void UILabel::SetText(const std::string &text)
  {
    Font font;
    if (_font)
    {
      font = *_font;
    }
    else
    {
      font = GetFontDefault();
    }
    this->text = text;
    _size = MeasureTextEx(font, text.c_str(), fontSize, 1);
  }

  void UILabel::Render()
  {
    Font font;
    if (_font)
    {
      font = *_font;
    }
    else
    {
      font = GetFontDefault();
    }

    DrawTextEx(font, text.c_str(), _position, fontSize, 1, WHITE);
  }
} // namespace Base
