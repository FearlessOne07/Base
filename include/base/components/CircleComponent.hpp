#pragma once
#include "base/components/Component.hpp"

namespace Base
{
  class CircleComponent : public Component
  {
  private:
    float _radius = 0;
    Color _color = WHITE;

  public:
    CircleComponent(float radius, Color color);
    float GetRadius() const;
    Color GetColor() const;
  };
} // namespace Base
