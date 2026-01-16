#pragma once
#include "base/ui/UIElement.hpp"
#include <string>

namespace Base
{
  class UIButton : public UIElement
  {
  private:
    std::wstring _text = L"Button";
    float _fontSize = 12;

    Color _backgroundColor = {255, 255, 255, 255};
    Color _textColor = {0, 0, 0, 0};

  public:
    void SetText(const std::wstring &);
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
