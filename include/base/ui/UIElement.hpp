#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/input/InputEvent.hpp"
#include "base/textures/Font.hpp"
#include "base/ui/UILayoutSettings.hpp"
#include <functional>
#include <memory>
#include <raylib.h>

namespace Base
{
  class UIElement
  {
    friend class UILayer;
    friend class UIContainer;

  protected:
    AssetHandle<BaseFont> _font;

    Vector2 _position = {0, 0};
    Vector2 _setPosition = {0, 0};
    Vector2 _size = {0, 0};
    bool _isHidden = false;

    std::function<void()> _onShow = nullptr;
    std::function<void()> _onHide = nullptr;

  protected:
    UILayoutSettings _layoutSettings;
    virtual void Update(float dt);

  public:
    virtual ~UIElement();

    // Setters
    void SetLayoutSettings(const UILayoutSettings &settings);
    void SetPosition(Vector2 position);
    void SetFont(const AssetHandle<BaseFont> &);
    void SetSize(Vector2 size);

    // Getters
    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    const UILayoutSettings &GetLayoutSettings() const;

    // Core
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event);
    virtual void Render() = 0;

    // Hide
    bool IsVisible() const;
    virtual void Show();
    virtual void Hide();
  };
} // namespace Base
