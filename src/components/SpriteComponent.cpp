#include "base/components/SpriteComponent.hpp"
#include "base/sprites/Sprite.hpp"

namespace Base
{
  SpriteComponent::SpriteComponent(const Sprite &sprite) : _sprite(sprite)
  {
  }

  void SpriteComponent::SetSourceRect(const Rectangle &rect)
  {
    _sprite.SetSourceRect(rect);
  }

  Vector2 SpriteComponent::GetTargetSize() const
  {
    return _sprite.GetTargetSize();
  }

  const Sprite &SpriteComponent::GetSprite() const
  {
    return _sprite;
  }
} // namespace Base
