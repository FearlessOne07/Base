#pragma once

#include "Component.hpp"

namespace Base
{
  class RigidBodyComponent : public Component
  {
  public:
    // General
    Vector2 direction = {0, 0};
    bool isKinematic = true;
    Vector2 velocity = {0, 0};

    // Non-Kinematics
    float mass = 1.f;
    float drag = 0.f;
    float friction = 0.f;
  };
} // namespace Base
