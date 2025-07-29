#include "base/components/SpriteComponent.hpp"

namespace Base
{

  SpriteComponent::SpriteComponent( //
    const AssetHandle<Texture> &textureHandle, const Vector2 &sourceIndex, const Vector2 &sourceSize,
    const Vector2 &destinationSize //
    )
    : _sprite(Sprite(textureHandle, sourceIndex, sourceSize, destinationSize))
  {
  }

  SpriteComponent::SpriteComponent(const Sprite &sprite) : _sprite(sprite)
  {
  }

  void SpriteComponent::SetSourceIndex(const Vector2 &source)
  {
    _sprite.SetSourceIndex(source);
  }

  const Rectangle SpriteComponent::GetTextureSourceRect() const
  {
    return _sprite.GetTextureSourceRect();
  }

  const Sprite &SpriteComponent::GetSprite() const
  {
    return _sprite;
  }

  const Vector2 SpriteComponent::GetTargetSize() const
  {
    return _sprite.GetTargetSize();
  }

  Vector2 SpriteComponent::GetSourceIndex() const
  {
    return _sprite.GetSourceIndex();
  }
} // namespace Base
