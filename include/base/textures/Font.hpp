#include "base/assets/BaseAsset.hpp"
#include "raylib.h"
#include <memory>

#pragma once
namespace Base
{
  class BaseFont : public BaseAsset
  {
    const std::shared_ptr<Font> _raylibFont;

  public:
    const std::shared_ptr<Font> &GetRaylibFont()
    {
      return _raylibFont;
    }
    BaseFont(Font font) : _raylibFont(std::make_shared<Font>(font))
    {
    }
  };
} // namespace Base
