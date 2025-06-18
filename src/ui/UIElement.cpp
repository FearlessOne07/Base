#include "base/ui/UIElement.hpp"
#include "base/ui/UILayoutSettings.hpp"

namespace Base
{
  UIElement::~UIElement()
  {
  }

  void UIElement::SetFont(const AssetHandle<BaseFont> &font)
  {
    if (font.Get())
    {
      _font = font;
    }
  }

  void UIElement::SetPosition(Vector2 position)
  {
    _setPosition = position;
    _position = position;
  }

  void UIElement::SetSize(Vector2 size)
  {
    _size = size;
  }

  void UIElement::SetLayoutSettings(const UILayoutSettings &settings)
  {
    _layoutSettings = settings;
  }

  Vector2 UIElement::GetPosition() const
  {
    return _setPosition;
  }

  Vector2 UIElement::GetSize() const
  {
    return _size;
  }

  const UILayoutSettings &UIElement::GetLayoutSettings() const
  {
    return _layoutSettings;
  }

  void UIElement::Show()
  {
    _isHidden = false;
    if (_onShow)
    {
      _onShow();
    }
  }

  void UIElement::Hide()
  {
    if (_onHide)
    {
      _onHide();
    }
    _isHidden = true;
  }

  bool UIElement::IsVisible() const
  {
    return !_isHidden;
  }

  void UIElement::Update(float dt)
  {
  }

  void UIElement::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
  }
} // namespace Base
