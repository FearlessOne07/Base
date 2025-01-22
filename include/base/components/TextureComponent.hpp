#pragma once
#include "base/Component.hpp"
#include "raylib.h"

namespace Base
{
  struct BASEAPI TextureComponent : public Component
  {
    Texture texture;
    float scale = 0.f;
    Vector2 size = {0, 0};
    Vector2 origin = {0, 0};
  };
} // namespace Base
