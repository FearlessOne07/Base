
#include "base/rendering/Quad.hpp"

namespace Base
{

  Rectangle::Rectangle(Vector2 size, Origin origin) : Renderable(), _size(size), _origin(origin)
  {
  }

  Rectangle::Rectangle(Vector2 size, const Material &material, Origin origin)
    : Renderable(material), _size(size), _origin(origin)
  {
  }

  Rectangle::Rectangle(Vector2 position, Vector2 size, Origin origin)
    : _position(position), _size(size), _origin(origin)
  {
  }

  Rectangle::Rectangle(Vector2 position, Vector2 size, const Material &material, Origin origin)
    : Renderable(material), _position(position), _size(size), _origin(origin)
  {
  }

  Vector2 Rectangle::GetSize() const
  {
    return _size;
  }

  Origin Rectangle::GetOrigin() const
  {
    return _origin;
  }

} // namespace Base
