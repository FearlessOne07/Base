#pragma once
#include "Component.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/sprites/Sprite.hpp"
#include "base/textures/Texture.hpp"
#include "raylib.h"

namespace Base
{
  class SpriteComponent : public Component
  {
  private:
    AssetHandle<Texture> _textureHandle;
    Vector2 _source = {0, 0};
    Vector2 _size{0, 0};
    Vector2 _targetSize{0, 0};

  public:
    SpriteComponent( //
      const AssetHandle<Texture> &textureHandle, const Vector2 &source, const Vector2 &sourceSize,
      const Vector2 &targetSize //
    );
    const Rectangle GetSourceRect() const;
    void SetSourceRect(const Rectangle &rect);
    const AssetHandle<Texture> &GetTexture() const;
    Vector2 GetTargetSize() const;
  };
} // namespace Base
