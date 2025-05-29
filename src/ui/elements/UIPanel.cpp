#include "base/ui/elements/UIPanel.hpp"
#include "raylib.h"

namespace Base
{
  void UIPanel::SetColor(Color color)
  {
    _color = color;
  }

  void UIPanel::Render()
  {
    DrawRectangleRounded({_position.x, _position.y, _size.x, _size.y}, 5, 100, _color);
  }
} // namespace Base
