#include "base/components/CircleComponent.hpp"

namespace Base
{
  CircleComponent::CircleComponent(const Circle &circle, Color color) : _circle(circle), _color(color)
  {
  }

  float CircleComponent::GetRadius() const
  {
    return _circle.GetRadius();
  }

  Color CircleComponent::GetColor() const
  {
    return _color;
  }
} // namespace Base
