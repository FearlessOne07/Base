#pragma once
#include "Component.hpp"
#include "base/sprites/Sprite.hpp"
#include "raylib.h"

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
