#include "base/components/SpriteComponent.hpp"
#include "base/sprites/Sprite.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  SpriteComponent::SpriteComponent(const Sprite &sprite) : _sprite(sprite)
  {
  }

  void SpriteComponent::SetSourcePos(const Vector2 source)
  {
    _sprite.SetSourcePos(source);
  }

  const Sprite &SpriteComponent::GetSprite() const
  {
    return _sprite;
  }
} // namespace Base
