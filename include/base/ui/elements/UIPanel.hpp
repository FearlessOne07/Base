#pragma once
#include "base/ui/UIElement.hpp"
namespace Base
{
  class UIPanel : public UIElement
  {
  private:
    Color _color = WHITE;

  public:
    void SetColor(Color color);
    void Render() override;
  };
} // namespace Base
