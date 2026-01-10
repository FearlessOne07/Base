#pragma once
#include "base/ui/UIElement.hpp"
namespace Base
{
  class UIPanel : public UIElement
  {
  private:
    Color _color = {255, 255, 255, 255};

  public:
    void SetColor(Color color);
    Color GetColor() const;
    void Render(float opacity) override;
  };
} // namespace Base
