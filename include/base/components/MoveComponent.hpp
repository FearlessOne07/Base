#pragma once
#include "Component.hpp"

namespace Base
{
  struct MoveComponent : public Component
  {
  public:
    float driveForce = 0.f;
    float brakeForce = 0.f;
  };
} // namespace Base
