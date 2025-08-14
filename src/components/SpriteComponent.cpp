#include "base/components/SpriteComponent.hpp"

namespace Base
{

  SpriteComponent::SpriteComponent( //
    const AssetHandle<Texture> &textureHandle, const Vector2 &sourcePosition, const Vector2 &sourceSize,
    const Vector2 &targetSize //
    )
    : _textureHandle(textureHandle), _source(sourcePosition), _size(sourceSize), _targetSize(targetSize)
  {
  }

  const Rectangle SpriteComponent::GetSourceRect() const
  {
    return {_source.x, _source.y, _size.x, _size.y};
  }

  const AssetHandle<Texture> &SpriteComponent::GetTexture() const
  {
    return _textureHandle;
  }

  void SpriteComponent::SetSourceRect(const Rectangle &rect)
  {
    _source = {rect.x, rect.y};
    _size = {rect.width, rect.height};
  }

  Vector2 SpriteComponent::GetTargetSize() const
  {
    return _targetSize;
  }
} // namespace Base
