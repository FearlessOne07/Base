#include "base/ui/elements/UIButton.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "raylib.h"

namespace Base
{
  void UIButton::Render()
  {
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, static_cast<float>(fontSize), 1);
    DrawRectangleLinesEx({position.x, position.y, textSize.x, textSize.y}, 2, WHITE);

    DrawTextEx(GetFontDefault(), text, position, fontSize, 1, WHITE);
  }

  void UIButton::Update()
  {
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, static_cast<float>(fontSize), 1);
    Vector2 mousePos = RenderContextSingleton::GetInstance()->GetScreenToGame(GetMousePosition());

    if (CheckCollisionPointRec(mousePos, {position.x, position.y, textSize.x, textSize.y}))
    {
      _isHovered = true;
    }
  }

  void UIButton::OnMouseButtonEvent(const std::shared_ptr<MouseButtonEvent> &event)
  {
    if (event->button == MOUSE_BUTTON_LEFT && event->action == MouseButtonEvent::Action::PRESSED)
    {
      if (_isHovered)
      {
        onClick();
        event->isHandled = true;
      }
    }
  }

} // namespace Base
