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

    Vector2 _basePosition = {0, 0};
    Vector2 _currentPosition = {0, 0};

    Vector2 _baseSize = {0, 0};
    Vector2 _currentSize = {0, 0};

    bool _isHidden = false;

  protected:
    UILayoutSettings _layoutSettings;
    virtual void Update(float dt);

  public:
    virtual ~UIElement();

    // Setters
    void SetLayoutSettings(const UILayoutSettings &settings);
    void SetFont(const AssetHandle<BaseFont> &);

    void SetPosition(Vector2 position, bool base = true);
    void SetSize(Vector2 size, bool base = true);

    // Getters
    Vector2 GetPosition() const;
    Vector2 GetBasePosition() const;

    Vector2 GetSize() const;
    Vector2 GetBaseSize() const;
    const UILayoutSettings &GetLayoutSettings() const;

    // Core
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event);
    virtual void Render() = 0;

    // Hide
    bool IsVisible() const;
    virtual void Show();
    virtual void Hide();
    std::function<void()> _onShow = nullptr;
    std::function<void()> _onHide = nullptr;
  };
} // namespace Base
