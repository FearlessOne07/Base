#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/Texture.hpp"
#include "base/rendering/Quad.hpp"
#include "base/util/Type.hpp"
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
    Vector2 _sourcePos = {0, 0};
    Vector2 _sourceSize = {0, 0};
    Rectangle _patches[9];
    float _borderScaleMultiplier = 1;

  public:
    NinePatchSprite( //
      const AssetHandle<Texture> &texture, Border border, Vector2 sourcePos, Vector2 sourceSize,
      float borderScaleMultiplier = 2.f //
    );

    NinePatchSprite() = default;
    void Draw(const Rectangle &dest, unsigned char alpha);
    void SetSourcePos(const Vector2 &pos);

    inline operator bool()
    {
      return _texture;
    }
  };
} // namespace Base
