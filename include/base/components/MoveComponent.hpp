#pragma once
#include "base/Component.hpp"
#include "raylib.h"

namespace Base
{
  class MoveComponent : public Component
  {
  public:
    Vector2 position = {0, 0};
    Vector2 targetVelocity = {0, 0};
    Vector2 velocity = {0, 0};
    float speed = 0.f;
  };
} // namespace Base
