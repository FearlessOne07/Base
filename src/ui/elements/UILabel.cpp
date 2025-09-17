#include "base/ui/elements/UILabel.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"
#include <string>

namespace Base
{
  void UILabel::SetText(const std::string &text)
  {
    _text = text;
  }

  Size UILabel::Measure()
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

    auto textSize = MeasureTextEx(font, _text.c_str(), _fontSize, 1);
    _desiredSize = {textSize.x, textSize.y};
    _desiredSize.width += _paddingLeft + _paddingRight;
    _desiredSize.height += _paddingTop + _paddingBottom;
    return _desiredSize;
  }

  void UILabel::Arrange(Rectangle finalRect)
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

    auto textSize = MeasureTextEx(font, _text.c_str(), _fontSize * _renderTransform.fontScale, 1);
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

  void UILabel::SetFontSize(float size)
  {
    _fontSize = size;
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
      font, _text.c_str(), {_layoutRect.x, _layoutRect.y}, _fontSize * _renderTransform.fontScale, 1,
      {
        _textColor.r,
        _textColor.g,
        _textColor.b,
        static_cast<unsigned char>(_alpha * _parentAlpha * 255),
      } //
    );
  }
} // namespace Base
