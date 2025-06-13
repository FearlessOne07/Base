#pragma once

#include "base/assets/BaseAsset.hpp"
#include "raylib.h"
#include <memory>
namespace Base
{
  class BaseShader : public BaseAsset
  {
    const std::shared_ptr<Shader> _raylibShader;

  public:
    const std::shared_ptr<Shader> &GetRaylibShader() const
    {
      return _raylibShader;
    }

    BaseShader(Shader shader) : _raylibShader(std::make_shared<Shader>(shader))
    {
    }
  };
} // namespace Base
