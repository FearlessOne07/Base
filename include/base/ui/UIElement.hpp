#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/input/InputEvent.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/textures/Font.hpp"
#include "base/util/AntagonisticFunction.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
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

  enum class SizeMode
  {
    Fixed = 0,
    Auto
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
    SizeMode _widthSizeMode = SizeMode::Auto;
    SizeMode _heightSizeMode = SizeMode::Auto;

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

  public:
    AntagonisticFunction onHover;
    std::function<void()> onClick = nullptr;
    std::function<void()> onShow = nullptr;
    std::function<void()> onHide = nullptr;

  public:
    virtual ~UIElement();

    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    void Update(float dt);
    // Setters
    void SetHAlignment(HAlign hAlign);
    void SetVAlignment(VAlign vAlign);
    void SetFont(const AssetHandle<BaseFont> &);

    void SetWidth(float width);
    void SetHeight(float height);
    void SetWidthSizeMode(SizeMode mode);
    void SetHeightSizeMode(SizeMode mode);

    void SetSizeMode(SizeMode mode);
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

    Size GetDesiredSize() const;
    Vector2 GetPosition() const;

    // New
    virtual Size Measure();
    virtual void Arrange(Rectangle finalRect);

    virtual void OnElementInputEvent(std::shared_ptr<InputEvent> &event);
    virtual void UpdateElement(float dt);

    // Render Tranform
    RenderTransform &GetRenderTransform();

    // Templates

  public:
    template <typename T> std::shared_ptr<T> AddChild(const std::string &name)
    {
      if (std::is_base_of_v<UIElement, T>)
      {
        std::string lower = Base::Strings::ToLower(name);
        if (std::ranges::find(_childElementIds, lower) == _childElementIds.end())
        {
          _childElements.emplace_back(std::make_shared<T>());
          _childElementIds.emplace_back(name);
          return std::static_pointer_cast<T>(_childElements.back());
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Element " + name + " already exists in container");
        }
      }
      else
      {
        THROW_BASE_RUNTIME_ERROR("T must be a derivative of UIElement");
      }
    }

    template <typename T> std::shared_ptr<T> GetChild(const std::string &name)
    {
      if (std::is_base_of_v<UIElement, T>)
      {
        std::string lower = Base::Strings::ToLower(name);
        if (auto it = std::ranges::find(_childElementIds, lower); it != _childElementIds.end())
        {
          auto index = std::distance(_childElementIds.begin(), it);
          return std::static_pointer_cast<T>(_childElements[index]);
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Element " + name + " does not exist in container");
        }
      }
      else
      {
        THROW_BASE_RUNTIME_ERROR("T must be a derivative of UIElement");
      }
    }
  };
} // namespace Base
