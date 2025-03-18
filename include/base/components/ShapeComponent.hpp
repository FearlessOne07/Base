#pragma once
#include "base/Component.hpp"
#include "base/Exports.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct BASEAPI ShapeComponent : public Component
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
