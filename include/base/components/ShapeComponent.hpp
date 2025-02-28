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
    unsigned int points = 3;
    Color color = WHITE;
    bool fill = true;
    float nonFillThickness = 1;
    float rotation = 0.f;
  };
} // namespace Base
