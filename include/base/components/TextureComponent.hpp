#pragma once
#include "Component.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/textures/Texture.hpp"
#include "raylib.h"

namespace Base
{
  struct TextureComponent : public Component
  {
    AssetHandle<Base::Texture> texture;
    Rectangle source = {0, 0, 0, 0};
    Vector2 targetSize = {0, 0};
  };
} // namespace Base
