#include "base/components/QuadComponent.hpp"

namespace Base
{
  QuadComponent::QuadComponent(Color color, Vector2 size) : _color(color), _size(size)
  {
  }

  Vector2 QuadComponent::GetSize() const
  {
    return _size;
  }

  Color QuadComponent::GetColor() const
  {
    return _color;
  }
} // namespace Base
