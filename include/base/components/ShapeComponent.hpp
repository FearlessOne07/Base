#pragma once
#include "Component.hpp"

namespace Base
{
  class ShapeComponent : public Component
  {
  public:
    float radius = 0.f;
    int points = 3;
    Color color = WHITE;
    bool fill = true;
    float nonFillThickness = 1;
    bool fillOutline = false;
    float outlineThickness = 3;
    Color outlineColor = BLACK;
  };
} // namespace Base
