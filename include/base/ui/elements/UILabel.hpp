#pragma once
#include "base/ui/UIElement.hpp"
#include <string>
namespace Base
{
  class UILabel : public UIElement
  {
  public:
    std::string text = "";
    float fontSize = 12;

    void SetText(const std::string &text);
    const std::string &GetText() const;

    void Render() override;
  };
} // namespace Base
