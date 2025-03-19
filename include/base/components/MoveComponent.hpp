#pragma once
#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct   MoveComponent : public Component
  {
  public:
    Vector2 targetVelocity = {0, 0};
    Vector2 velocity = {0, 0};
    float acceleration = 0;
    float speed = 0.f;
  };
} // namespace Base
