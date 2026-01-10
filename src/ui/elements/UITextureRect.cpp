#include "base/ui/elements/UITextureRect.hpp"
#include "base/util/Draw.hpp"

namespace Base
{
  void UITextureRect::Render(float opacity)
  {
    if (!_isHidden)
    {
      if (_sprite)
      {
        _sprite.Draw( //
          {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height},
          _renderTransform.GetOpacity() * opacity * 255 //
        );
      }
      else
      {
        DrawRectangleBase( //
          {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height}, {0, 0}, 0,
          {255, 255, 255, static_cast<unsigned char>(_renderTransform.GetOpacity() * opacity * 255)} //
        );
      }
    }
  }
} // namespace Base
