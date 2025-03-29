#pragma once

#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct ImpulseComponent : public Component
  {
    Vector2 direction = {0, 0};
    float force = 0;
    float forceDecayFactor = 0;
  };
} // namespace Base
