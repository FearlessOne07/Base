#pragma once
#include "base/ui/UIElement.hpp"
#include "raylib.h"
#include <string>

namespace Base
{
  class UIButton : public UIElement
  {
  private:
    std::string _text = "Button";
    float _fontSize = 12;

    Color _normalColor = WHITE;
    Color _hoverColor = GRAY;
    Color _activeColor = GRAY;
    Color _textColor = BLACK;
    Color _color = _normalColor;

  public:
    void SetText(const std::string &);
    void SetColors(Color hoverColor, Color activeColor, Color normalColor, Color textColor);
    void SetFontSize(float size);
    float GetFontSize() const;

    // Core
    void Render(float opacity) override;
    void UpdateElement(float dt) override;

    // New
    Size Measure() override;
    void Arrange(Rectangle finalRect) override;
  };
} // namespace Base
