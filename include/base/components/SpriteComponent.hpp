#pragma once
#include "Component.hpp"
#include "base/rendering/Sprite.hpp"

namespace Base
{
  class SpriteComponent : public Component
  {
  private:
    Sprite _sprite;

  public:
    SpriteComponent(const Sprite &sprite);
    const Sprite &GetSprite() const;
    void SetSourcePos(const Vector2 source);
    void SetSourceSize(const Vector2 size);
  };
} // namespace Base
