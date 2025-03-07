#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct BASEAPI BoundingBoxComponent : public Component
  {
    Vector2 size = {0, 0};
    Vector2 positionOffset = {0, 0};
    Vector2 lastPosition = {0, 0};

    bool draw = false;
    bool fill = true;
    Color color = WHITE;
    float nonFillThickness = 3;
  };
} // namespace Base
