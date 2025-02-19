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
    Color color = WHITE;
    bool fill = true;
    float nonFillThickness = 0;
    float rotation = 0.f;
  };
} // namespace Base
