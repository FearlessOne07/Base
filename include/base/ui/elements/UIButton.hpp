#pragma once
#include "base/ui/UIElement.hpp"
#include <functional>
#include <memory>

namespace Base
{
  class UIButton : public UIElement
  {
  private:
    void Render() override;
    void Update() override;

    void OnMouseButtonEvent(const std::shared_ptr<MouseButtonEvent> &event) override;

  private:
    bool _isHovered = false;

  public:
    float fontSize = 30;
    const char *text = "Button";
    std::function<void()> onClick = nullptr;
  };
} // namespace Base
