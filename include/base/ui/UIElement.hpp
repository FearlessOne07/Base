#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/input/InputEvent.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/textures/Font.hpp"
#include "base/ui/UILayoutSettings.hpp"
#include "base/util/AntagonisticFunction.hpp"
#include <algorithm>
#include <functional>
#include <memory>
#include <raylib.h>

namespace Base
{
  struct Size
  {
    float width = 0, height = 0;
  };

  class RenderTransform
  {
    float _scaleX = 1, _scaleY = 1;
    float _fontScale = 1;
    float _offsetX = 0, _offsetY = 0;
    float _opactity = 1;

  public:
    float GetScaleX() const
    {
      return _scaleX;
    }

    float GetScaleY() const
    {
      return _scaleY;
    }

    float GetFontScale() const
    {
      return _fontScale;
    }

    float GetOffsetx() const
    {
      return _offsetX;
    }

    float GetOffsetY() const
    {
      return _offsetY;
    }

    float GetOpacity() const
    {
      return _opactity;
    }

    void SetOpacity(float value)
    {
      _opactity = std::clamp(value, 0.f, 1.f);
    }

    void SetOffsetX(float value)
    {
      _offsetX = value;
    }

    void SetOffsetY(float value)
    {
      _offsetY = value;
    }

    void SetFontScale(float value)
    {
      _fontScale = value;
    }

    void SetScaleX(float value)
    {
      _scaleX = value;
    }

    void SetScaleY(float value)
    {
      _scaleY = value;
    }
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

    void SetSize(Size size);
    void SetSprite(const NinePatchSprite &sprite);

    // Padding
    void SetPadding(float padding);
    void SetPadding(float paddingX, float paddingY);
    void SetPadding(float paddingLeft, float paddingRight, float paddingTop, float paddingBottom);

    // Core
    virtual void Render(float alpha) = 0;

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
    Vector2 GetPosition() const;

    virtual void OnElementInputEvent(std::shared_ptr<InputEvent> &event);
    virtual void UpdateElement(float dt);

    // Render Tranform
    RenderTransform &GetRenderTransform();
  };
} // namespace Base
