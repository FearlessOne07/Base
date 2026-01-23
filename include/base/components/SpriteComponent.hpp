#pragma once
#include "Component.hpp"
#include "base/rendering/Sprite.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  class SpriteComponent : public Component
  {
  private:
    Sprite _sprite;
    Vector2 _targetSize;

  public:
    SpriteComponent(const Sprite &sprite, Vector2 targetSize);
    const Sprite &GetSprite() const;
    void SetSourcePos(const Vector2 source);
    void SetSourceSize(const Vector2 size);
    Vector2 GetTargetSize() const;
  };
} // namespace Base
