#include "base/components/CircleComponent.hpp"

namespace Base
{
  CircleComponent::CircleComponent(float radius, Color color) : _radius(radius), _color(color)
  {
  }

  float CircleComponent::GetRadius() const
  {
    return _radius;
  }

  Color CircleComponent::GetColor() const
  {
    return _color;
  }
} // namespace Base
