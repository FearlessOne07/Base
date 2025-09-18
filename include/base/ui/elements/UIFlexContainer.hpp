#pragma once
#include "base/ui/UIElement.hpp"
namespace Base
{
  class UIFlexContainer : public UIElement
  {
  public:
    enum struct Orientation : uint8_t
    {
      Vertical,
      Horizontal
    };

  private:
    Orientation _orientation = Orientation::Horizontal;
    Color _backgroundColor = BLANK;

  public:
    Size Measure() override;
    void Arrange(Rectangle finalRect) override;
    void Render(float alpha) override;
    void SetBackgroundColor(Color color);
    void SetOrientation(Orientation orientation);
  };
} // namespace Base
