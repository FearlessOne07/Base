#include "base/ui/UIElement.hpp"

namespace Base
{
  class UICanvas : public UIElement
  {
    Color _fillColor = {0, 0, 0, 0};

  public:
    void Arrange(Rectangle finalRect) override;
    void Render(float alpha) override;
    void SetFillColor(Color);
  };
} // namespace Base
