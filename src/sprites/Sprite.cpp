#include "base/sprites/Sprite.hpp"
#include "base/rendering/Origin.hpp"
#include "internal/rendering/Renderer.hpp"

namespace Base
{
  Sprite::Sprite( //
    const AssetHandle<Texture> &textureHandle, const Vector2 &sourceIndex, const Vector2 &sourceSize,
    const Vector2 &destinationSize //
    )
    : _texture(textureHandle), _sourcePos(sourceIndex), _sourceSize(sourceSize), _targetSize(destinationSize)
  {
  }

  // Copy constructor
  Sprite::Sprite(const Sprite &other)
    : _texture(other._texture), _sourcePos(other._sourcePos), _sourceSize(other._sourceSize),
      _targetSize(other._targetSize)
  {
  }

  // Move constructor
  Sprite::Sprite(Sprite &&other) noexcept
    : _texture(std::move(other._texture)), _sourcePos(std::move(other._sourcePos)),
      _sourceSize(std::move(other._sourceSize)), _targetSize(std::move(other._targetSize))
  {
  }

  // Copy assignment operator
  Sprite &Sprite::operator=(const Sprite &other)
  {
    if (this != &other)
    {
      _texture = other._texture;
      _sourcePos = other._sourcePos;
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
      _sourcePos = std::move(other._sourcePos);
      _sourceSize = std::move(other._sourceSize);
      _targetSize = std::move(other._targetSize);
    }
    return *this;
  }

  void Sprite::SetTargetSize(Vector2 size)
  {
    _targetSize = size;
  }

  void Sprite::SetSourceRect(const Rectangle &rect)
  {
    _sourcePos = {rect.GetPosition().x, rect.GetPosition().y};
    _sourceSize = {rect.GetSize().x, rect.GetSize().y};
  }

  Vector2 Sprite::GetTargetSize() const
  {
    return _targetSize;
  }

  void Sprite::Render(Vector2 position, float rotatation, Color tint) const
  {
    Renderer::DrawSprite( //
      Sprite{_texture.Get(), _sourcePos, _sourceSize, Origin::Center}, position, _targetSize,
      {Base::FramebufferAttachmentIndex::Color0, Base::FramebufferAttachmentIndex::Color1} //
    );

    DrawTexturePro( //
      *_texture.Get()->GetRaylibTexture(), {_sourcePos.x, _sourcePos.y, _sourceSize.x, _sourceSize.y},
      {position.x, position.y, _targetSize.x, _targetSize.y}, {_targetSize.x / 2, _targetSize.y / 2}, rotatation,
      {
        static_cast<unsigned char>((tint.r * tint.a) / 255),
        static_cast<unsigned char>((tint.g * tint.a) / 255),
        static_cast<unsigned char>((tint.b * tint.a) / 255),
        tint.a,
      } //
    );
  }
} // namespace Base
