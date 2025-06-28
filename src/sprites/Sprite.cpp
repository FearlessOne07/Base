#include "base/sprites/Sprite.hpp"
#include "raylib.h"

namespace Base
{

  Sprite::Sprite( //
    const AssetHandle<Texture> &textureHandle, const Vector2 &sourceIndex, const Vector2 &sourceSize,
    const Vector2 &destinationSize //
    )
    : _texture(textureHandle), _sourceIndex(sourceIndex), _sourceSize(sourceSize), _targetSize(destinationSize)
  {
  }

  void Sprite::SetSourceIndex(const Vector2 &source)
  {
    _sourceIndex = source;
  }

  Vector2 Sprite::GetSourceIndex() const
  {
    return _sourceIndex;
  }

  Vector2 Sprite::GetSourceSize() const
  {
    return _sourceSize;
  }

  const Rectangle Sprite::GetTextureSourceRect() const
  {
    return {
      _sourceIndex.x * _sourceSize.x,
      _sourceIndex.y * _sourceSize.y,
      _sourceSize.x,
      _sourceSize.y,
    };
  }

  const AssetHandle<Texture> &Sprite::GetTexture() const
  {
    return _texture;
  }

  const Vector2 Sprite::GetTargetSize() const
  {
    return _targetSize;
  }
} // namespace Base
