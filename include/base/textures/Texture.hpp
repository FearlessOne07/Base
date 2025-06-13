#include "base/assets/BaseAsset.hpp"
#include "raylib.h"
#include <memory>
#pragma once
namespace Base
{
  class Texture : public BaseAsset
  {
    const std::shared_ptr<Texture2D> _raylibTexture;

  public:
    const std::shared_ptr<Texture2D> &GetRaylibTexture() const
    {
      return _raylibTexture;
    }

    Texture(Texture2D shader) : _raylibTexture(std::make_shared<Texture2D>(shader))
    {
    }
  };
} // namespace Base
