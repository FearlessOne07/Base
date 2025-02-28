#pragma once
#include "base/Component.hpp"
#include "raylib/raylib.h"

namespace Base
{

  struct TransformComponent : public Component
  {
    Vector2 position = {0, 0};
    float rotation = 0;
  };

} // namespace Base
