#include "base/ui/elements/UIButton.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"
#include <memory>

namespace Base
{

  void UIButton::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
  }

  void UIButton::SetColors(Color hoverColor, Color activeColor, Color normalColor, Color textColor)
  {
    _textColor = textColor;
    _hoverColor = hoverColor;
    _activeColor = activeColor;
    _normalColor = normalColor;
  }

  void UIButton::SetText(const std::string &text)
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

  void UIButton::SetFontSize(float size, bool base)
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

  void UIButton::SetFont(const AssetHandle<BaseFont> &fontHandle)
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

  void UIButton::Render()
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
      _sprite.Draw({GetPosition().x, GetPosition().y, _currentSize.x, _currentSize.y}, _alpha * _parentAlpha * 255);
    }
    else
    {
      DrawRectangleRec( //
        {GetPosition().x, GetPosition().y, _currentSize.x, _currentSize.y},
        {_color.r, _color.g, _color.b, static_cast<unsigned char>(_alpha * _parentAlpha * 255)} //
      );
    }

    // Measure text size
    Vector2 textSize = MeasureTextEx(font, _text.c_str(), _currentFontSize, 1);

    // Center text inside padded button area
    Vector2 targetPos = {GetPosition().x + _currentSize.x / 2, GetPosition().y + _currentSize.y / 2};
    Vector2 textPos = {targetPos.x - textSize.x / 2, targetPos.y - textSize.y / 2};
    DrawTextBase( //
      font, _text.c_str(), textPos, _currentFontSize, 1,
      {
        _textColor.r,
        _textColor.g,
        _textColor.b,
        static_cast<unsigned char>(_alpha * _parentAlpha * 255),
      } //
    );
  }

  void UIButton::SetPadding(Vector2 padding)
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

  void UIButton::Update(float dt)
  {
    _color = _isHovered ? _hoverColor : _normalColor;
  }

  float UIButton::GetFontSize() const
  {
    return _currentFontSize;
  }

  float UIButton::GetBaseFontSize() const
  {
    return _baseFontSize;
  }
} // namespace Base
