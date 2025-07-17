#include "base/ui/elements/UITextureRect.hpp"
#include "raylib.h"

namespace Base
{
  void UITextureRect::Render()
  {

    if (_sprite)
    {
      _sprite.Draw({GetPosition().x, GetPosition().y, _currentSize.x, _currentSize.y});
    }
    else
    {
      DrawRectangleRec({GetPosition().x, GetPosition().y, _currentSize.x, _currentSize.y}, WHITE);
    }
  }
} // namespace Base
