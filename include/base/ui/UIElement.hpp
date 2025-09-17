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
  struct Size
  {
    float width = 0, height = 0;
  };

  struct RenderTransform
  {
    float scaleX = 1, scaleY = 1;
    float fontScale = 1;
  };

  enum class HAlign
  {
    Left,
    Center,
    Right,
    Stretch
  };

  enum class VAlign
  {
    Top,
    Center,
    Bottom,
    Stretch
  };

  class UIElement
  {
  protected:
    Rectangle GetCombinedHoverRect() const;

  protected:
    AssetHandle<BaseFont> _font;
    NinePatchSprite _sprite;

    Size _desiredSize = {0, 0};

    std::vector<std::string> _childElementIds;
    std::vector<std::shared_ptr<UIElement>> _childElements;

    Rectangle _layoutRect = {0, 0, 0, 0};

    // Padding
    float _paddingLeft = 0, _paddingRight = 0, _paddingTop = 0, _paddingBottom = 0;

    bool _isHidden = false;

    float _alpha = 1;
    float _parentAlpha = 1;

    HAlign _horizontalAlignment = HAlign::Left;
    VAlign _verticalAlignment = VAlign::Top;

    // Render
    RenderTransform _renderTransform;

    // Hover
    bool _isHovered = false;
    bool _firstHover = false;
    bool _isActive = false;

    // Template Functions
  protected:
  public:
    AntagonisticFunction onHover;
    std::function<void()> onClick = nullptr;

  protected:
    UILayoutSettings _layoutSettings;

  public:
    virtual ~UIElement();

    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    void Update(float dt);
    // Setters
    void SetHAlignment(HAlign hAlign);
    void SetVAlignment(VAlign vAlign);
    void SetFont(const AssetHandle<BaseFont> &);
    virtual void SetAlpha(float alpha);
    virtual void SetParentAlpha(float alpha);

    void SetSize(Size size);
    void SetSprite(const NinePatchSprite &sprite);

    // Padding
    void SetPadding(float padding);
    void SetPadding(float paddingX, float paddingY);
    void SetPadding(float paddingLeft, float paddingRight, float paddingTop, float paddingBottom);

    // Core
    virtual void Render() = 0;

    // Hide
    bool IsVisible() const;
    virtual void Show();
    virtual void Hide();
    void SetVisibilityOff();
    std::function<void()> onShow = nullptr;
    std::function<void()> onHide = nullptr;

  public:
    // New
    virtual Size Measure();
    virtual void Arrange(Rectangle finalRect);
    Size GetDesiredSize() const;

    virtual void OnElementInputEvent(std::shared_ptr<InputEvent> &event);
    virtual void UpdateElement(float dt);

    // Render Tranform
    void SetRenderTransform(const RenderTransform &transform);
    const RenderTransform &GetRenderTransform() const;
  };
} // namespace Base
