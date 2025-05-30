#include "base/ui/elements/UIButton.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/renderer/RenderContext.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "raylib.h"
#include <memory>

namespace Base
{

  void UIButton::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    if (auto mouseEvent = std::dynamic_pointer_cast<Base::MouseButtonEvent>(event))
    {
      if (                                                                                                            //
        mouseEvent->action == Base::InputEvent::Action::HELD && mouseEvent->button == MOUSE_BUTTON_LEFT && _isHovered //
      )
      {
        _isActive = true;
        event->isHandled = true;
      }
      else if ( //
        mouseEvent->action == Base::InputEvent::Action::RELEASED && mouseEvent->button == MOUSE_BUTTON_LEFT &&
        _isActive //
      )
      {
        onClick();
        _isActive = false;
        event->isHandled = true;
      }
      else
      {
        _isActive = false;
      }
    }
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
    if (_font)
    {
      font = *_font;
    }
    else
    {
      font = GetFontDefault();
    }
    _text = text;
    _size = MeasureTextEx(font, text.c_str(), _fontSize, 1);

    _size.x += _padding.x * 2;
    _size.y += _padding.y * 2;
  }

  void UIButton::SetFontSize(float size)
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
    _fontSize = size;
    _size = MeasureTextEx(font, _text.c_str(), _fontSize, 1);

    _size.x += _padding.x * 2;
    _size.y += _padding.y * 2;
  }

  void UIButton::Render()
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
    DrawRectangleRec({_position.x, _position.y, _size.x, _size.y}, _color);
    DrawTextEx(font, _text.c_str(), {_position.x + _padding.x, _position.y + _padding.y}, _fontSize, 1, _textColor);
  }

  void UIButton::SetPadding(Vector2 padding)
  {
    _padding = padding;
  }

  void UIButton::Update(float dt)
  {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    Vector2 mousePos = rd->mousePosition;

    bool isCurrentlyHovered = CheckCollisionPointRec(mousePos, {_position.x, _position.y, _size.x, _size.y});

    if (isCurrentlyHovered && !_isHovered)
    {
      // Mouse just entered the button
      if (onHover)
      {
        onHover.flex();
      }
    }
    else if (!isCurrentlyHovered && _isHovered)
    {
      // Mouse just exited the button
      if (onHover)
      {
        onHover.relax();
      }
    }

    _isHovered = isCurrentlyHovered;
    _color = _isHovered ? _hoverColor : _normalColor;
  }

  float UIButton::GetFontSize() const
  {
    return _fontSize;
  }
} // namespace Base
