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
    Vector2 velocity = {0, 0};

    // Kinematic
    float speed = 0.f;
    Vector2 acceleration = {0, 0};

    // Non-Kinematics
    float mass = 1.f;
    float drag = 0.f;
    float friction = 0.f;
  };
} // namespace Base
