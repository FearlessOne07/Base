#pragma once
#include "base/input/InputEvent.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include <algorithm>
#include <cctype>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace Base
{
  class UILayer
  {
  private:
    std::vector<std::string> _elementIds;
    std::vector<std::shared_ptr<UIElement>> _elements;
    bool _isHidden = false;

  public:
    template <typename T> std::shared_ptr<T> AddElement(const std::string &id)
    {
      if (!std::is_base_of_v<UIElement, T>)
      {
        THROW_BASE_RUNTIME_ERROR("T must ba a derivative of UIElement");
      }

      std::string lowerid = Base::Strings::ToLower(id);

      if (std::ranges::find(_elementIds, lowerid) != _elementIds.end())
      {
        THROW_BASE_RUNTIME_ERROR("Element " + id + " already registered in layer");
      }

      _elementIds.emplace_back(lowerid);
      _elements.emplace_back(std::make_shared<T>());

      return std::static_pointer_cast<T>(_elements.back());
    }

    template <typename T> std::shared_ptr<T> GetElement(const std::string &id)
    {
      if (!std::is_base_of_v<UIElement, T>)
      {
        THROW_BASE_RUNTIME_ERROR("T must ba a derivative of UIElement");
      }

      std::string lowerid = id;
      std::ranges::transform(id, lowerid.begin(), [](char c) { return std::tolower(c); });

      if (auto it = std::ranges::find(_elementIds, lowerid); it != _elementIds.end())
      {
        int index = static_cast<int>(std::distance(_elementIds.begin(), it));
        return std::static_pointer_cast<T>(_elements[index]);
      }
      else
      {
        THROW_BASE_RUNTIME_ERROR("Element " + id + " isn't registerd in layer");
      }
    }

    void RemoveElement(const std::string &id);
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    void Render();
    void Update(float dt);
    bool IsVisible() const;
    void Hide();
    void Show();
  };
} // namespace Base
