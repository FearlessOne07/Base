#pragma once
#include "base/ui/UIElement.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include "raylib.h"
#include <algorithm>
#include <memory>
#include <string>

namespace Base
{
  class UIContainer : public UIElement
  {
  public:
    enum struct Layout : uint8_t
    {
      VERTICAL = 0,
      HORIZONTAL
    };

    enum AnchorPoint : uint8_t
    {
      TOP_LEFT,
      TOP_CENTER,
      TOP_RIGHT,
      RIGHT_CENTER,
      BOTTOM_RIGHT,
      BOTTOM_CENTER,
      BOTTOM_LEFT,
      LEFT_CENTER,
      CENTER
    };

  private:
    // UI Elements
    std::vector<std::string> _childElementIds;
    std::vector<std::shared_ptr<UIElement>> _childElements;

    // Layout
    AnchorPoint _anchorPoint = AnchorPoint::TOP_LEFT;
    Layout _layout = Layout::VERTICAL;
    bool _dirty = false;
    Vector2 _padding = {0, 0};
    float _gapSize = 0;
    bool _sizeCalculated = false;

  private:
    void LayoutVertical();
    void LayoutHorizontal();
    void UpdatePosition();

  public:
    // Layout
    void SetPadding(Vector2 padding);
    void SetGapSize(float gapSize);
    void SetLayout(Layout layout);
    void SetAnchorPoint(AnchorPoint anchorPoint);

    // Core
    void Update(float dt) override;
    void Render() override;

    void Hide() override;
    void Show() override;
    void OnInputEvent(std::shared_ptr<InputEvent> &event) override;

  public:
    // Templates
    template <typename T> std::shared_ptr<T> AddChild(const std::string &name)
    {
      if (std::is_base_of_v<UIElement, T>)
      {
        std::string lower = Base::Strings::ToLower(name);
        if (std::ranges::find(_childElementIds, lower) == _childElementIds.end())
        {
          _childElements.emplace_back(std::make_shared<T>());
          _childElementIds.emplace_back(name);
          _dirty = true;
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
  };
} // namespace Base
