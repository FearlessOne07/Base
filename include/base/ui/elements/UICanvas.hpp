#include "base/ui/UIElement.hpp"
#include "raylib.h"

namespace Base
{
  class UICanvas : public UIElement
  {
    Color _fillColor = BLANK;

  public:
    void Arrange(Rectangle finalRect) override;
    void Render(float alpha) override;
    void SetFillColor(Color);
  };
} // namespace Base
