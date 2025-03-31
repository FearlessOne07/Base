#pragma once

#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{
  struct ImpulseComponent : public Component
  {
    Vector2 direction = {0, 0};
    float force = 0;
    float duration = 1.f;
    float elapsedTime = 0.f;

    bool IsActive() const;
  };
} // namespace Base
