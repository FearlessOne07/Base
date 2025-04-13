#pragma once
#include "base/ui/UIElement.hpp"

namespace Base
{
  class UILabel : public UIElement
  {
  private:
    void Render() override;

  public:
    const char *text = "Label";
    float fontSize = 30;
  };
} // namespace Base
