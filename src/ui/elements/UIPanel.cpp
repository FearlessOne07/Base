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
    float alpha = opacity * _renderTransform.GetOpacity();
    if (!_isHidden && alpha > 0)
    {
      Color color = {_color.r, _color.g, _color.b, 255 * alpha};
      Renderer::DrawQuad(_layoutRect, _layoutRect.GetPosition(), color);
    }
  }
} // namespace Base
