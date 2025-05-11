#pragma once
#include "base/ui/UIElement.hpp"
#include <string>
namespace Base
{
  class UILabel : public UIElement
  {
    std::string _text = "Label";
    float _fontSize = 12;

  public:
    void SetText(const std::string &text);
    void SetFontSize(float size);
    const std::string &GetText() const;

    void Render() override;
  };
} // namespace Base
