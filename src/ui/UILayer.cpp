#include "base/ui/UILayer.hpp"
#include <ranges>

namespace Base
{
  void UILayer::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    for (auto &element : _elements)
    {
      element->OnInputEvent(event);

      if (event->isHandled)
      {
        break;
      }
    }
  }
  void UILayer::Render()
  {
    for (auto &element : std::ranges::reverse_view(_elements))
    {
      element->Render();
    }
  }

  void UILayer::RemoveElement(const std::string &id)
  {

    std::string lowerid = id;
    std::ranges::transform(id, lowerid.begin(), [](char c) { return std::tolower(c); });

    if (auto it = std::ranges::find(_elementIds, lowerid); it != _elementIds.end())
    {
      int index = static_cast<int>(std::distance(_elementIds.begin(), it));
      _elementIds.erase(it);
      _elements.erase(_elements.begin() + index);
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Element " + id + " isn't registerd in layer");
    }
  }

  void UILayer::Update(float dt)
  {
    for (auto &element : _elements)
    {
      element->_update(dt);
    }
  }
} // namespace Base
