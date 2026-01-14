#include "base/rendering/Sprite.hpp"

namespace Base
{

  Sprite::Sprite(std::shared_ptr<Texture> texture, Origin origin) : Renderable(), _texture(texture), _origin(origin)
  {
    _sourceSize = {_texture->GetWidth(), _texture->GetHeight()};
  }

  Sprite::Sprite(std::shared_ptr<Texture> texture, glm::vec2 sourcePos, glm::vec2 sourceSize, Origin origin)
    : Renderable(), _texture(texture), _sourcePosition(sourcePos), _sourceSize(sourceSize), _origin(origin)
  {
  }

  Sprite::Sprite( //
    std::shared_ptr<Texture> texture, glm::vec2 sourcePos, glm::vec2 sourceSize, const Material &material,
    Origin origin //
    )
    : Renderable(material), _texture(texture), _sourcePosition(sourcePos), _sourceSize(sourceSize), _origin(origin)
  {
  }

  Sprite::Sprite(std::shared_ptr<Texture> texture, const Material &material, Origin origin)
    : Renderable(material), _texture(texture), _origin(origin)
  {
    _sourceSize = {_texture->GetWidth(), _texture->GetHeight()};
  }
  glm::vec2 Sprite::GetSourceSize() const
  {
    return _sourceSize;
  }

  glm::vec2 Sprite::GetSourcePos() const
  {
    return _sourcePosition;
  }

  std::shared_ptr<Texture> Sprite::GetTexture() const
  {
    return _texture;
  }

  Origin Sprite::GetOrigin() const
  {
    return _origin;
  }

  void Sprite::SetSourceSize(const Vector2 size)
  {
    _sourceSize = size;
  }

  void Sprite::SetSourcePos(const Vector2 pos)
  {
    _sourcePosition = pos;
  }

  Sprite::operator bool()
  {
    return _texture != nullptr;
  }
} // namespace Base
