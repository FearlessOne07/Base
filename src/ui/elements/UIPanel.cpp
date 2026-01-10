#include "base/ui/elements/UIPanel.hpp"
#include "base/util/Draw.hpp"

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

  void UIPanel::Render(float opacity)
  {

    if (!_isHidden)
    {
      DrawRectangleBase( //
        {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height}, {0, 0}, 0,
        {
          _color.r,
          _color.g,
          _color.b,
          static_cast<unsigned char>(_renderTransform.GetOpacity() * opacity * 255),
        } //
      );
    }
  }
} // namespace Base
