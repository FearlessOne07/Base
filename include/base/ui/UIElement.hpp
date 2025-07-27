#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/input/InputEvent.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/textures/Font.hpp"
#include "base/ui/UILayoutSettings.hpp"
#include "base/util/AntagonisticFunction.hpp"
#include <functional>
#include <memory>
#include <raylib.h>

namespace Base
{
  class UIElement
  {
  public:
    enum struct ElementSizeMode : uint8_t
    {
      FIXED = 0,
      FIT
    };

    enum struct ContainerSizeMode : uint8_t
    {
      DEFAULT = 0,
      FILL
    };

    friend class UILayer;
    friend class UIContainer;

  private:
    Vector2 _basePosition = {0, 0};
    Vector2 _layoutPosition = {0, 0};
    Vector2 _positionalOffset = {0, 0};

  private:
    virtual void Update(float dt);
    Rectangle GetCombinedHoverRect() const;

    // Template Methods
    void _update(float dt);
    void _onInputEvent(std::shared_ptr<InputEvent> &event);

  protected:
    AssetHandle<BaseFont> _font;
    NinePatchSprite _sprite;

    Vector2 _baseSize = {0, 0};
    Vector2 _currentSize = {0, 0};
    ElementSizeMode _elementSizeMode = ElementSizeMode::FIT;
    ContainerSizeMode _containerSizeMode = ContainerSizeMode::DEFAULT;

    bool _isHidden = false;

    bool _isHovered = false;
    bool _firstHover = false;

    bool _isActive = false;

    float _alpha = 1;
    float _parentAlpha = 1;

  public:
    AntagonisticFunction onHover;
    std::function<void()> onClick = nullptr;

  protected:
    UILayoutSettings _layoutSettings;

  public:
    virtual ~UIElement();

    // Setters
    void SetLayoutSettings(const UILayoutSettings &settings);
    virtual void SetFont(const AssetHandle<BaseFont> &);
    virtual void SetAlpha(float alpha);
    virtual void SetParentAlpha(float alpha);

    void SetPosition(Vector2 position, bool final = false);
    void SetPositionalOffset(Vector2 offset);
    void SetSize(Vector2 size, bool base = true);
    void SetContainterSizeMode(ContainerSizeMode sizeMode);
    void SetElementSizeMode(ElementSizeMode sizeMode);
    void SetSprite(const NinePatchSprite &sprite);

    // Getters
    Vector2 GetPosition() const;
    Vector2 GetPositionalOffset() const;
    float GetAlpha() const;

    Vector2 GetSize() const;
    Vector2 GetBaseSize() const;
    ContainerSizeMode GetContainerSizeMode() const;
    ElementSizeMode GetElementSizeMode() const;
    const UILayoutSettings &GetLayoutSettings() const;

    // Core
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event);
    virtual void Render() = 0;

    // Hide
    bool IsVisible() const;
    virtual void Show();
    virtual void Hide();
    void SetVisibilityOff();
    std::function<void()> _onShow = nullptr;
    std::function<void()> _onHide = nullptr;
  };
} // namespace Base
