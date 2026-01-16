#pragma once
#include "base/ui/Binding.hpp"
#include "base/ui/UIElement.hpp"
#include <string>
namespace Base
{
  class UILabel : public UIElement
  {
    std::wstring _text = L"Label";
    std::wstring _cachedText = L"Label";
    Binding<std::wstring> _textBinding;
    Color _textColor = {255, 255, 255, 255};
    float _fontSize = 12;

    void SetTextInternal(const std::wstring &, bool user);

  public:
    void SetText(const std::wstring &text);
    void Bind(const Binding<std::wstring> &binding);
    void ClearBinding();
    void SetFontSize(float size);
    void SetTextColor(Color color);
    void Render(float opacity) override;
    const std::wstring &GetText() const;

    // New
    Size Measure() override;
    void Arrange(Rectangle finalRect) override;

    void UpdateElement(float dt, UIContext uiContext) override;
  };
} // namespace Base
