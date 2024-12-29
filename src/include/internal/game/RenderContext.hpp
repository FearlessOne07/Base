#pragma once

#include "raylib.h"
namespace Base
{

struct RenderContext
{
  float gameWidth;
  float gameHeight;
  float marginX;
  float marginY;
  float scale;
  mutable Camera2D camera;

  Vector2 GetScreenToGame(Vector2 vec) const
  {
    vec.x = (vec.x - marginX) / scale;
    vec.y = (vec.y - marginY) / scale;
    return vec;
  }
};

} // namespace Base
