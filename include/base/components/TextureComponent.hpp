#pragma once
#include "base/Component.hpp"
#include "raylib.h"
#include <memory>

namespace Base
{
  struct BASEAPI TextureComponent : public Component
  {
    std::shared_ptr<Texture> texture;
    float scale = 1.f;
    Rectangle source = {0, 0, 0, 0};
    Vector2 origin = {0, 0};
  };
} // namespace Base
