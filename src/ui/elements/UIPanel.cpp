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

    if (!_isHidden)
    {
      Renderer::DrawQuad(_layoutRect, _layoutRect.GetPosition(), _color);
    }
  }
} // namespace Base
