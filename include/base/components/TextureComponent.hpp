#pragma once
#include "Component.hpp"
#include "raylib.h"
#include <memory>

namespace Base
{
  struct TextureComponent : public Component
  {
    std::shared_ptr<Texture> texture;
    Rectangle source = {0, 0, 0, 0};
  };
} // namespace Base
