#include "base/components/CircleComponent.hpp"

namespace Base
{
  CircleComponent::CircleComponent(const Circle &circle, Color color) : _circle(circle), _color(color)
  {
  }

  const Circle &CircleComponent::GetCircle() const
  {
    return _circle;
  }

  const Color &CircleComponent::GetColor() const
  {
    return _color;
  }
} // namespace Base
