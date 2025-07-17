#pragma once

#include "base/ui/UIElement.hpp"
#include "raylib.h"
namespace Base
{
  class UITextureRect : public UIElement
  {
  private:
  public:
    void Render() override;
  };
} // namespace Base
