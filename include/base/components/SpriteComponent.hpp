#pragma once
#include "Component.hpp"
#include "base/rendering/Quad.hpp"
#include "base/sprites/Sprite.hpp"

namespace Base
{
  class SpriteComponent : public Component
  {
  private:
    Sprite _sprite;

  public:
    SpriteComponent(const Sprite & sprite);
    const Sprite &GetSprite() const;
    Vector2 GetTargetSize() const;
    void SetSourceRect(const Rectangle &rect);
  };
} // namespace Base
