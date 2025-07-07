#include "base/ui/elements/UIPanel.hpp"
#include "raylib.h"

namespace Base
{
  void UIPanel::SetColor(Color color)
  {
    _color = color;
  }

  Color UIPanel::GetColor() const
  {
    return _color;
  }

  void UIPanel::Render()
  {
    DrawRectangleRec({GetPosition().x, GetPosition().y, _currentSize.x, _currentSize.y}, _color);
  }
} // namespace Base
