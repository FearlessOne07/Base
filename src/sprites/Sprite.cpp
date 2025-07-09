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

  // Copy constructor
  Sprite::Sprite(const Sprite &other)
    : _texture(other._texture), _sourceIndex(other._sourceIndex), _sourceSize(other._sourceSize),
      _targetSize(other._targetSize)
  {
  }

  // Move constructor
  Sprite::Sprite(Sprite &&other) noexcept
    : _texture(std::move(other._texture)), _sourceIndex(std::move(other._sourceIndex)),
      _sourceSize(std::move(other._sourceSize)), _targetSize(std::move(other._targetSize))
  {
  }

  // Copy assignment operator
  Sprite &Sprite::operator=(const Sprite &other)
  {
    if (this != &other)
    {
      _texture = other._texture;
      _sourceIndex = other._sourceIndex;
      _sourceSize = other._sourceSize;
      _targetSize = other._targetSize;
    }
    return *this;
  }

  // Move assignment operator
  Sprite &Sprite::operator=(Sprite &&other) noexcept
  {
    if (this != &other)
    {
      _texture = std::move(other._texture);
      _sourceIndex = std::move(other._sourceIndex);
      _sourceSize = std::move(other._sourceSize);
      _targetSize = std::move(other._targetSize);
    }
    return *this;
  }

  void Sprite::SetSourceIndex(const Vector2 &source)
  {
    _sourceIndex = source;
  }

  void Sprite::SetDestinationSize(const Vector2 &size)
  {
    _targetSize = size;
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
