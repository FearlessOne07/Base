#pragma once
#include "base/ui/UIElement.hpp"
#include <functional>

namespace Base
{
  class UIButton : public UIElement
  {
  private:
    void Render() override;
    void Update() override;

  private:
    bool _isHovered = false;

  public:
    int fontSize = 30;
    const char *text = "Button";
    std::function<void()> onClick = nullptr;
  };
} // namespace Base
