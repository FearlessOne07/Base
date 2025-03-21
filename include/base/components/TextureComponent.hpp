#pragma once
#include "base/Component.hpp"
#include "raylib/raylib.h"
#include <memory>

namespace Base
{
  struct   TextureComponent : public Component
  {
    std::shared_ptr<Texture> texture;
    float scale = 1.f;
    Rectangle source = {0, 0, 0, 0};
    Vector2 origin = {0, 0};
  };
} // namespace Base
