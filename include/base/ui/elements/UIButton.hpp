#pragma once
#include "base/ui/UIElement.hpp"
#include "base/util/AntagonisticFunction.hpp"
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
    float _fontSize = 12;
    Vector2 _padding = {10, 10};

    Color _normalColor = WHITE;
    Color _hoverColor = GRAY;
    Color _activeColor = GRAY;
    Color _textColor = BLACK;
    Color _color = _normalColor;

    bool _isHovered = false;
    bool _firstHover = false;

  public:
    AntagonisticFunction onHover;

  public:
    std::function<void()> onClick = nullptr;
    void SetText(const std::string &);
    void SetColors(Color hoverColor, Color activeColor, Color normalColor, Color textColor);
    void SetFontSize(float);
    void SetPadding(Vector2 padding);
    void Render() override;
    void Update(float dt) override;
    void OnInputEvent(std::shared_ptr<InputEvent> &event) override;

    float GetFontSize() const;
  };
} // namespace Base
