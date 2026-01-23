
#include "base/util/Circle.hpp"

namespace Base
{
  Circle::Circle(float radius) : _radius(radius)
  {
  }

  Circle::Circle(Vector2 position, float radius) : _position(position), _radius(radius), _thickness(radius)
  {
  }

  Circle::Circle(Vector2 position, float radius, const Material &material)
    : Renderable(material), _position(position), _radius(radius), _thickness(radius)
  {
  }

  Circle::Circle(Vector2 position, float radius, float thickness)
    : _position(position), _radius(radius), _thickness(thickness)
  {
  }

  Circle::Circle(Vector2 position, float radius, float thickness, const Material &material)
    : Renderable(material), _position(position), _radius(radius), _thickness(thickness)
  {
  }

  float Circle::GetRadius() const
  {
    return _radius;
  }

  Vector2 Circle::GetPosition() const
  {
    return _position;
  }
} // namespace Base
