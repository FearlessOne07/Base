#include "base/ui/elements/UITextureRect.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"

namespace Base
{
  void UITextureRect::Render()
  {
    if (_sprite)
    {
      _sprite.Draw({GetPosition().x, GetPosition().y, GetSize().x, GetSize().y}, _alpha * _parentAlpha * 255);
    }
    else
    {
      DrawRectangleBase( //
        {GetPosition().x, GetPosition().y, GetSize().x, GetSize().y}, {0, 0}, 0,
        {255, 255, 255, static_cast<unsigned char>(_alpha * _parentAlpha * 255)} //
      );
    }
  }
} // namespace Base
