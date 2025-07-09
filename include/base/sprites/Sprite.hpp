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
    Sprite(const Sprite &other);
    Sprite(Sprite &&other) noexcept;
    Sprite &operator=(const Sprite &other);
    Sprite &operator=(Sprite &&other) noexcept;

    // Setters/ Getters
    void SetSourceIndex(const Vector2 &source);
    void SetDestinationSize(const Vector2 &size);
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
