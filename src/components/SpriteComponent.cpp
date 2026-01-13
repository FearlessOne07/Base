#include "base/components/SpriteComponent.hpp"
#include "base/rendering/Sprite.hpp"
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

  void SpriteComponent::SetSourceSize(const Vector2 size)
  {
    _sprite.SetSourceSize(size);
  }

  const Sprite &SpriteComponent::GetSprite() const
  {
    return _sprite;
  }
} // namespace Base
