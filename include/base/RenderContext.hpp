#pragma once
#include "base/Exports.hpp"
#include "raylib/raylib.h"

namespace Base
{

  struct BASEAPI RenderContext
  {
    float gameWidth;
    float gameHeight;
    float marginX;
    float marginY;
    float scale;
    mutable Camera2D camera;

    // Functions
    Vector2 GetScreenToGame(Vector2 vec) const;
  };

} // namespace Base
