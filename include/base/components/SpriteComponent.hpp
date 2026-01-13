#pragma once
#include "Component.hpp"
#include "base/sprites/Sprite.hpp"

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
  };
} // namespace Base
