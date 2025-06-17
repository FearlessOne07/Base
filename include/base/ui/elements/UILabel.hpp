#pragma once
#include "base/ui/UIElement.hpp"
#include "raylib.h"
#include <string>
namespace Base
{
  class UILabel : public UIElement
  {
    std::string _text = "Label";
    Color _textColor = WHITE;
    float _fontSize = 12;

  public:
    void SetText(const std::string &text);
    void SetFontSize(float size);
    void SetTextColor(Color color);
    void Render() override;
    const std::string &GetText() const;
  };
} // namespace Base
