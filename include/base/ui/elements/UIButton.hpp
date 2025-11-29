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

    Color _backgroundColor = WHITE;
    Color _textColor = BLACK;

  public:
    void SetText(const std::string &);
    void SetTextColor(Color textColor);
    void SetBackgroundColor(Color color);

    void SetFontSize(float size);
    float GetFontSize() const;

    // Core
    void Render(float opacity) override;

    // New
    Size Measure() override;
    void Arrange(Rectangle finalRect) override;
  };
} // namespace Base
