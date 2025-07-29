#pragma once
#include "Component.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/sprites/Sprite.hpp"
#include "base/textures/Texture.hpp"
#include "raylib.h"

namespace Base
{
  struct SpriteComponent : public Component
  {
  private:
    Sprite _sprite;

  public:
    SpriteComponent( //
      const AssetHandle<Texture> &textureHandle, const Vector2 &sourceIndex, const Vector2 &sourceSize,
      const Vector2 &destinationSize //
    );
    SpriteComponent(const Sprite &sprite);
    void SetSourceIndex(const Vector2 &source);
    Vector2 GetSourceIndex() const;
    const Rectangle GetTextureSourceRect() const;
    const Vector2 GetTargetSize() const;
    const Sprite &GetSprite() const;
  };
} // namespace Base
