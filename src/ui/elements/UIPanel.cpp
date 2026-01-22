#include "base/ui/elements/UIPanel.hpp"
#include "internal/rendering/Renderer.hpp"

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

    if (!_isHidden && opacity > 0)
    {
      Color color = {_color.r, _color.g, _color.b, _color.a * opacity * _renderTransform.GetOpacity()};
      Renderer::DrawQuad(_layoutRect, _layoutRect.GetPosition(), color);
    }
  }
} // namespace Base
