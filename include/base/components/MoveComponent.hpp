#pragma once
#include "Component.hpp"

namespace Base
{
  class MoveComponent : public Component
  {
  public:
    float driveForce = 0.f;
    float brakeForce = 0.f;

    // Kinematic
    float speed = 0;
  };
} // namespace Base
