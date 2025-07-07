#include "base/ui/elements/UITextureRect.hpp"
#include "raylib.h"

namespace Base
{
  void UITextureRect::SetTextureHandle(const AssetHandle<Texture> &handle)
  {
    _texturehandle = handle;
  }

  void UITextureRect::SetSourceRect(Rectangle rect)
  {
    _soureceRect = rect;
  }

  void UITextureRect::Render()
  {
    DrawTexturePro( //
      *_texturehandle.Get()->GetRaylibTexture(), _soureceRect,
      {GetPosition().x, GetPosition().y, _currentSize.x, _currentSize.y}, {0, 0}, 0, WHITE //
    );
  }
} // namespace Base
