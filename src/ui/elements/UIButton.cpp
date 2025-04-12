#include "base/ui/elements/UIButton.hpp"
#include "raylib.h"

namespace Base
{
  void UIButton::Render()
  {
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, static_cast<float>(fontSize), 1);
    DrawRectangleLinesEx({position.x, position.y, textSize.x, textSize.y}, 2, WHITE);
  }

  void UIButton::Update()
  {
  }
} // namespace Base
