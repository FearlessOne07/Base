#pragma once
#include "base/components/Component.hpp"
#include "base/util/Circle.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  class CircleComponent : public Component
  {
  private:
    Circle _circle;
    Color _color = {255, 255, 255, 255};

  public:
    CircleComponent(const Circle &circle, Color color);
    const Circle &GetCircle() const;
    const Color &GetColor() const;
  };
} // namespace Base
