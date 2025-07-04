#pragma once

#include "base/assets/AssetHandle.hpp"
#include "base/textures/Texture.hpp"
#include "raylib.h"
namespace Base
{
  class Sprite
  {
  private:
    AssetHandle<Texture> _texture;
    Vector2 _sourceIndex = {0, 0};
    Vector2 _sourceSize = {0, 0};
    Vector2 _targetSize = {0, 0};

  public:
    Sprite() = default;
    Sprite( //
      const AssetHandle<Texture> &textureHandle, const Vector2 &sourceIndex, const Vector2 &sourceSize,
      const Vector2 &destinationSize //
    );

    void SetSourceIndex(const Vector2 &source);
    Vector2 GetSourceIndex() const;
    Vector2 GetSourceSize() const;
    const Rectangle GetTextureSourceRect() const;
    const Vector2 GetTargetSize() const;
    const AssetHandle<Texture> &GetTexture() const;

    // Converters
    inline operator bool()
    {
      return _texture;
    }
  };
} // namespace Base
