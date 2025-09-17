#pragma once
#include "base/ui/UIElement.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include "raylib.h"
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <memory>
#include <string>

namespace Base
{
  class UIStackPanel : public UIElement
  {
  public:
    enum struct Orientation : uint8_t
    {
      Vertical,
      Horizontal
    };

  private:
    // Layout
    Orientation _orientation = Orientation::Vertical;
    bool _dirty = false;
    float _gap = 10;
    Color _backgroundColor = BLANK;

  public:
    // Layout
    void SetGap(float gap);
    void SetOrientation(Orientation layout);
    void SetBackgroundColor(Color color);

    Color GetBackgroundColor() const;

    void SetAlpha(float alpha) override;
    void SetParentAlpha(float alpha) override;

    // Core
    void Render() override;

    void Hide() override;
    void Show() override;
    void OnElementInputEvent(std::shared_ptr<InputEvent> &event) override;

    // New
    Size Measure() override;
    void Arrange(Rectangle finalRect) override;

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
          _childElements.back()->SetParentAlpha(_alpha * _parentAlpha);
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
