#include "base/ui/elements/UIPanel.hpp"
#include "base/util/Draw.hpp"
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
    DrawRectangleBase( //
      {GetPosition().x, GetPosition().y, GetSize().x, GetSize().y}, {0, 0}, 0,
      {
        _color.r,
        _color.g,
        _color.b,
        static_cast<unsigned char>(_alpha * _parentAlpha * 255),
      } //
    );
  }
} // namespace Base
