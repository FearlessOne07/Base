#pragma once
#include "base/ui/UIElement.hpp"
#include <functional>

namespace Base
{
  class UIButton : UIElement
  {
  private:
    void Render() override;

  private:
    bool _isHovered = false;

  public:
    const char *text = "Button";
    std::function<void()> onClick = nullptr;
  };
} // namespace Base
