#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/textures/Texture.hpp"
#include "raylib.h"
namespace Base
{
  class NinePatchSprite
  {
  public:
    struct Border
    {
      float top = 0, bottom = 0;
      float left = 0, right = 0;
    };

  private:
    AssetHandle<Texture> _texture;
    Border _border;
    Vector2 _sourceIndex = {0, 0};
    Vector2 _sourceSize = {0, 0};
    Rectangle _patches[9];

  public:
    NinePatchSprite(const AssetHandle<Texture> &texture, Border border, Vector2 sourceIndex, Vector2 sourceSize);
    NinePatchSprite() = default;
    void Draw(Rectangle dest);

    inline operator bool()
    {
      return _texture;
    }
  };
} // namespace Base
