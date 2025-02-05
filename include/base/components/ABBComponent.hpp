#include "base/Component.hpp"
#include "raylib.h"

namespace Base
{
  struct BASEAPI ABBComponent : public Component
  {
    Rectangle boundingBox = {0, 0, 0, 0};

    bool draw = false;
    bool fill = true;
    Color color = WHITE;
    float nonFillThickness = 0;
  };
} // namespace Base
