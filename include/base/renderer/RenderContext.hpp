#pragma once
#include "raylib.h"

namespace Base
{
  struct RenderContext
  {
    float gameWidth = 0.f;
    float gameHeight = 0.f;
    float marginX = 0.f;
    float marginY = 0.f;
    float scale = 0.f;
    Vector2 mousePosition = {0, 0};
  };

} // namespace Base
