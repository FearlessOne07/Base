#include "base/ui/elements/UITextureRect.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"

namespace Base
{
  void UITextureRect::Render()
  {
    if (_sprite)
    {
      _sprite.Draw({_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height}, _alpha * _parentAlpha * 255);
    }
    else
    {
      DrawRectangleBase( //
        {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height}, {0, 0}, 0,
        {255, 255, 255, static_cast<unsigned char>(_alpha * _parentAlpha * 255)} //
      );
    }
  }
} // namespace Base
