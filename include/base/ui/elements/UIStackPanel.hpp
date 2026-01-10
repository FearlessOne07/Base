#pragma once
#include "base/ui/UIElement.hpp"
#include <cstdint>

namespace Base
{
  class UIStackPanel : public UIElement
  {
  public:
    enum struct Orientation : uint8_t
    {
      Vertical,
      Horizontal
    };

  private:
    // Layout
    Orientation _orientation = Orientation::Vertical;
    float _gap = 0;
    Color _backgroundColor = BLANK;

  public:
    // Layout
    void SetGap(float gap);
    void SetOrientation(Orientation layout);
    void SetBackgroundColor(Color color);

    // Core
    void Render(float opacity) override;

    void Hide() override;
    void Show() override;

    // New
    Size Measure() override;
    void Arrange(Rectangle finalRect) override;
  };
} // namespace Base
