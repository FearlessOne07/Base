#pragma once
#include "base/ui/UIElement.hpp"
#include "raylib.h"
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
