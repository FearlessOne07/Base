#pragma once

#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct RigidBodyComponent : public Component
  {
    // General
    Vector2 direction = {0, 0};
    bool isKinematic = true;

    // Kinematic
    float speed = 0.f;
    Vector2 acceleration = {0, 0};
    Vector2 velocity = {0, 0};
  };
} // namespace Base
