#pragma once
#include "base/Component.hpp"
#include "base/Exports.hpp"
#include "raylib.h"

namespace Base
{
  struct BASEAPI MoveComponent : public Component
  {
  public:
    Vector2 position = {0, 0};
    Vector2 targetVelocity = {0, 0};
    Vector2 velocity = {0, 0};
    int acceleration = 0;
    float speed = 0.f;

    MoveComponent(Vector2 position, Vector2 targetVelocity, Vector2 velocity, int acceleration, int speed)
      : position(position), targetVelocity(targetVelocity), velocity(velocity), acceleration(acceleration), speed(speed)
    {
    }
  };
} // namespace Base
