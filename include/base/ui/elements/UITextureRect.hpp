#pragma once

#include "base/ui/UIElement.hpp"
namespace Base
{
  class UITextureRect : public UIElement
  {
  public:
    void Render(float opacity) override;
  };
} // namespace Base
