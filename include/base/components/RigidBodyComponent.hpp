#pragma once
#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct RigidBodyComponent : public Component
  {
    float mass = 0.f;
    float coeffFricition = 0.f;

    float acceleration = 0.f;
    float force = 0.f;
    Vector2 direction = {0, 0};

    bool isUnderGravity = false;
  };
} // namespace Base
