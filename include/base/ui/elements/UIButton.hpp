#pragma once
#include "base/ui/UIElement.hpp"
#include <functional>
#include <string>

namespace Base
{
  class UIButton : public UIElement
  {
  private:
    bool _isHovered = false;
    bool _isActive = true;
    std::string _text = "Button";
    float _fontSize = 12;
    Vector2 _padding = {10, 10};

  public:
    std::function<void()> onClick = nullptr;
    void SetText(const std::string &);
    void SetFontSize(float);
    void SetPadding(Vector2 padding);
    void OnInputEvent(std::shared_ptr<InputEvent> &event) override;
    void Render() override;
    void Update(float dt) override;
  };
} // namespace Base
