#include "base/ui/elements/UILabel.hpp"
#include "raylib.h"

namespace Base
{
  void UILabel::Render()
  {
    DrawTextEx(GetFontDefault(), text, position, fontSize, 1, WHITE);
  }
} // namespace Base
