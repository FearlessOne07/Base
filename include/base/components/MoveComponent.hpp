#pragma once
#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct MoveComponent : public Component
  {
  public:
    float speed = 0.f;
    Vector2 velocity = {0, 0};
  };
} // namespace Base
