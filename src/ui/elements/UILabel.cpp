#include "base/ui/elements/UILabel.hpp"
#include "base/util/Draw.hpp"
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

    if (_elementSizeMode == ElementSizeMode::FIT)
    {
      _baseSize = MeasureTextEx(font, text.c_str(), _baseFontSize, 1);
      _baseSize.x += _padding.x * 2;
      _baseSize.y += _padding.y * 2;

      _currentSize = MeasureTextEx(font, _text.c_str(), _currentFontSize, 1);
      _currentSize.x += _padding.x * 2;
      _currentSize.y += _padding.y * 2;
    }
  }

  void UILabel::SetFontSize(float size, bool base)
  {
    Font font;
    if (_font)
    {
      font = *_font.Get()->GetRaylibFont();
    }
    else
    {
      font = GetFontDefault();
    }

    if (base)
    {
      _baseFontSize = size;
      _baseSize = MeasureTextEx(font, _text.c_str(), _baseFontSize, 1);
      _baseSize.x += _padding.x * 2;
      _baseSize.y += _padding.y * 2;
    }

    _currentFontSize = size;
    _currentSize = MeasureTextEx(font, _text.c_str(), _currentFontSize, 1);
    _currentSize.x += _padding.x * 2;
    _currentSize.y += _padding.y * 2;
  }

  void UILabel::SetFont(const AssetHandle<BaseFont> &fontHandle)
  {
    _font = fontHandle;
    Font font = *_font.Get()->GetRaylibFont();

    _baseSize = MeasureTextEx(font, _text.c_str(), _baseFontSize, 1);
    _baseSize.x += _padding.x * 2;
    _baseSize.y += _padding.y * 2;
    _currentSize = MeasureTextEx(font, _text.c_str(), _currentFontSize, 1);
    _currentSize.x += _padding.x * 2;
    _currentSize.y += _padding.y * 2;
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

    DrawTextBase( //
      font, _text.c_str(), GetPosition(), _currentFontSize, 1,
      {
        _textColor.r,
        _textColor.g,
        _textColor.b,
        static_cast<unsigned char>(_alpha * _parentAlpha * 255),
      } //
    );
  }

  void UILabel::SetPadding(Vector2 padding)
  {
    _padding = padding;
    Font font;
    if (_font.Get())
    {
      font = *_font.Get()->GetRaylibFont();
    }
    else
    {
      font = GetFontDefault();
    }

    _baseSize = MeasureTextEx(font, _text.c_str(), _baseFontSize, 1);
    _baseSize.x += _padding.x * 2;
    _baseSize.y += _padding.y * 2;
  }
} // namespace Base
