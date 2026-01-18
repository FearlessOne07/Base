#include "base/ui/elements/UILabel.hpp"
#include "base/ui/UIElement.hpp"
#include "internal/rendering/Renderer.hpp"
#include <string>

namespace Base
{
  void UILabel::SetText(const std::string &text)
  {
    _text = text;
  }

  void UILabel::SetTextInternal(const std::string &text, bool user = false)
  {
    _text = text;
    if (user && _textBinding)
    {
      _textBinding.Set(text);
    }
  }

  void UILabel::ClearBinding()
  {
    _textBinding = {};
  }

  void UILabel::Bind(const Binding<std::string> &binding)
  {
    _textBinding = std::move(binding);
  }

  void UILabel::UpdateElement(float dt, UIContext uiContext)
  {
    if (_textBinding)
    {
      std::string newText = _textBinding.Get();
      if (newText != _cachedText)
      {
        SetTextInternal(newText);
        _cachedText = newText;
      }
    }
  }

  Size UILabel::Measure()
  {
    auto textSize = Font::MeasureText(_font.Get(), _text, _fontSize);
    _desiredSize = {textSize.x, textSize.y};
    _desiredSize.width += _paddingLeft + _paddingRight;
    _desiredSize.height += _paddingTop + _paddingBottom;
    return _desiredSize;
  }

  void UILabel::Arrange(Rectangle finalRect)
  {
    auto textSize = Font::MeasureText(_font.Get(), _text, _fontSize * _renderTransform.GetFontScale());

    _layoutRect = finalRect;
    float width = textSize.x;
    float height = textSize.y;
    width += _paddingLeft + _paddingRight;
    height += _paddingTop + _paddingBottom;

    Vector2 pos(0);
    pos.x += _renderTransform.GetOffsetx();
    pos.y += _renderTransform.GetOffsetY();

    // Horizontal alignment
    switch (_horizontalAlignment)
    {
    case HAlign::Left:
      break;
    case HAlign::Center:
      pos.x += (finalRect.GetSize().x - width) / 2;
      break;
    case HAlign::Right:
      pos.x += finalRect.GetSize().x - width;
      break;
    case HAlign::Stretch:
      width = finalRect.GetSize().x;
      break;
    }

    // Vertical alignment
    switch (_verticalAlignment)
    {
    case VAlign::Top:
      break;
    case VAlign::Center:
      pos.y += (finalRect.GetSize().y - height) / 2;
      break;
    case VAlign::Bottom:
      pos.y += finalRect.GetSize().y - height;
      break;
    case VAlign::Stretch:
      height = finalRect.GetSize().y;
      break;
    }

    _layoutRect = {pos, {width, height}};
  }

  void UILabel::SetFontSize(float size)
  {
    _fontSize = size;
  }

  void UILabel::SetTextColor(Color color)
  {
    _textColor = color;
  }

  void UILabel::Render(float opacity)
  {

    if (!_isHidden)
    {
      Renderer::DrawText( //
          _text, _layoutRect.GetPosition(), _textColor, _fontSize * _renderTransform.GetFontScale(),
          _font.Get() //
      );
    }
  }
} // namespace Base
