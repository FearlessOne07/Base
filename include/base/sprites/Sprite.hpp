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
    Vector2 _sourcePos = {0, 0};
    Vector2 _sourceSize = {0, 0};
    Vector2 _targetSize = {0, 0};

  public:
    Sprite() = default;
    Sprite( //
      const AssetHandle<Texture> &textureHandle, const Vector2 &sourcePos, const Vector2 &sourceSize,
      const Vector2 &destinationSize //
    );
    Sprite(const Sprite &other);
    Sprite(Sprite &&other) noexcept;
    Sprite &operator=(const Sprite &other);
    Sprite &operator=(Sprite &&other) noexcept;

    // Converters
    inline operator bool()
    {
      return _texture;
    }

    void Render(Vector2 position, float rotatation, Color tint) const;
    void SetTargetSize(Vector2 size);
    void SetSourceRect(const Rectangle &rect);
    Vector2 GetTargetSize() const;
  };
} // namespace Base
