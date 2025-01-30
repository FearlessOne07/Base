#pragma once
#include "base/Component.hpp"
#include "base/Exports.hpp"
#include "raylib.h"

namespace Base
{
  struct BASEAPI ShapeComponent : public Component
  {
  public:
    float radius = 0.f;
    unsigned int points = 0;
    Color color = {255, 255, 255, 255};
    bool fill = true;
    float nonFillThickness = 0;
    float rotation = 0.f;

    ShapeComponent(float radius, unsigned int points, Color color, bool fill, float nonFillThickness, float rotation)
      : radius(radius), points(points), color(color), fill(fill), nonFillThickness(nonFillThickness), rotation(rotation)
    {
    }
  };
} // namespace Base
