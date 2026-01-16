#include "base/ui/elements/UITextureRect.hpp"
#include "internal/rendering/Renderer.hpp"

namespace Base
{
  void UITextureRect::Render(float opacity)
  {
    if (!_isHidden)
    {
      if (_sprite)
      {
        _sprite.Draw( //
          _layoutRect,
          _renderTransform.GetOpacity() * opacity * 255 //
        );
      }
      else
      {
        Renderer::DrawQuad(_layoutRect, _layoutRect.GetPosition(), {255, 255, 255, 255});
      }
    }
  }
} // namespace Base
