#include "base/components/QuadComponent.hpp"
#include "base/rendering/Quad.hpp"

namespace Base
{
  QuadComponent::QuadComponent(const Rectangle &rectanlge, Color color) : _color(color), _rectangle(rectanlge)
  {
  }

  const Rectangle &QuadComponent::GetRectangle() const
  {
    return _rectangle;
  }

  const Color &QuadComponent::GetColor() const
  {
    return _color;
  }
} // namespace Base
