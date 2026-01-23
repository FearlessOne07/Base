#pragma once
#include "base/ui/Binding.hpp"
#include "base/ui/UIElement.hpp"
#include <string>
namespace Base
{
  class UILabel : public UIElement
  {
    std::string _text = "Label";
    std::string _cachedText = "Label";
    Binding<std::string> _textBinding;
    Color _textColor = {255, 255, 255, 255};
    float _fontSize = 12;

    void SetTextInternal(const std::string &, bool user);

  public:
    void SetText(const std::string &text);
    void Bind(const Binding<std::string> &binding);
    void ClearBinding();
    void SetFontSize(float size);
    void SetTextColor(Color color);
    void Render(float opacity) override;
    const std::string &GetText() const;

    // New
    Size Measure() override;
    void Arrange(Rectangle finalRect) override;

    void UpdateElement(float dt, UIContext uiContext) override;
  };
} // namespace Base
