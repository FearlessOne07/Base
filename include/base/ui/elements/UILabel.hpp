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

    float _baseFontSize = 12;
    float _currentFontSize = 12;

    Vector2 _padding = {0, 0};

  public:
    void SetText(const std::string &text);
    void SetFont(const AssetHandle<BaseFont> &) override;
    void SetFontSize(float size, bool base = true);
    void SetPadding(Vector2 padding);
    void SetTextColor(Color color);
    void Render() override;
    const std::string &GetText() const;
  };
} // namespace Base
