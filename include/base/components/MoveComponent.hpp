#pragma once
#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct MoveComponent : public Component
  {
  public:
    float driveForce = 0.f;
    float brakeForce = 0.f;
  };
} // namespace Base
