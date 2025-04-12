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

    DrawText(text, (int)position.x, (int)position.y, fontSize, WHITE);
  }

  void UIButton::Update()
  {
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, static_cast<float>(fontSize), 1);
    Vector2 mousePos = RenderContextSingleton::GetInstance()->GetScreenToGame(GetMousePosition());

    if (CheckCollisionPointRec(mousePos, {position.x, position.y, textSize.x, textSize.y}))
    {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      {
        onClick();
      }
    }
  }
} // namespace Base
