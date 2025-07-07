#pragma once
#include "base/ui/UIElement.hpp"
#include "raylib.h"
#include <functional>
#include <string>

namespace Base
{
  class UIButton : public UIElement
  {
  private:
    bool _isActive = false;
    std::string _text = "Button";
    Vector2 _padding = {10, 10};

    float _baseFontSize = 12;
    float _currentFontSize = 12;

    Color _normalColor = WHITE;
    Color _hoverColor = GRAY;
    Color _activeColor = GRAY;
    Color _textColor = BLACK;
    Color _color = _normalColor;

  public:
    std::function<void()> onClick = nullptr;
    void SetText(const std::string &);
    void SetColors(Color hoverColor, Color activeColor, Color normalColor, Color textColor);
    void SetFontSize(float size, bool base = true);
    void SetPadding(Vector2 padding);
    void Render() override;
    void Update(float dt) override;
    void OnInputEvent(std::shared_ptr<InputEvent> &event) override;
    float GetFontSize() const;
    float GetBaseFontSize() const;
  };
} // namespace Base
