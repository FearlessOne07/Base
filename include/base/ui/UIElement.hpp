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
    void _update(float dt);
    virtual void Update(float dt);
    Rectangle GetCombinedHoverRect() const;

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

  public:
    AntagonisticFunction onHover;

  protected:
    UILayoutSettings _layoutSettings;

  public:
    virtual ~UIElement();

    // Setters
    void SetLayoutSettings(const UILayoutSettings &settings);
    void SetFont(const AssetHandle<BaseFont> &);

    void SetPosition(Vector2 position);
    void SetPositionalOffset(Vector2 offset);
    void SetSize(Vector2 size, bool base = true);
    void SetContainterSizeMode(ContainerSizeMode sizeMode);
    void SetElementSizeMode(ElementSizeMode sizeMode);
    void SetSprite(const NinePatchSprite &sprite);

    // Getters
    Vector2 GetPosition() const;
    Vector2 GetPositionalOffset() const;

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
