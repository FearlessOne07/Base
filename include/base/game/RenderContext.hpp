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
    mutable Camera2D camera = {
      .offset = {0, 0},
      .target = {0, 0},
      .rotation = 0,
      .zoom = 1,
    };

    // Functions
    Vector2 GetScreenToGame(Vector2 vec) const;
  };

} // namespace Base
