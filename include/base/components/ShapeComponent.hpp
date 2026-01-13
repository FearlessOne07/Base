#pragma once
#include "Component.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  class ShapeComponent : public Component
  {
  public:
    float radius = 0.f;
    int points = 3;
    Color color = {255, 255, 255, 255};
    bool fill = true;
    float nonFillThickness = 1;
    bool fillOutline = false;
    float outlineThickness = 3;
    Color outlineColor = {0, 0, 0, 255};
  };
} // namespace Base
