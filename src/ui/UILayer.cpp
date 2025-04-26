#include "base/ui/UILayer.hpp"
#include "base/Exception.hpp"
namespace Base
{

  void UILayer::Add(const std::string &elementId, const std::shared_ptr<UIElement> &element)
  {
    if (_elements.find(elementId) == _elements.end())
    {
      _elements[elementId] = element;
    }
  }

  void UILayer::Remove(const std::string &elementId)
  {
    if (_elements.find(elementId) == _elements.end())
    {
      THROW_BASE_RUNTIME_ERROR("Specified UI element does not exist");
    }
    else
    {

      auto it = _elements.find(elementId);
      _elements.erase(it);
    }
  }

  std::shared_ptr<UIElement> &UILayer::Get(const std::string &elementId)
  {
    if (_elements.find(elementId) == _elements.end())
    {
      THROW_BASE_RUNTIME_ERROR("Specified UI element does not exist");
    }
    else
    {
      return _elements.at(elementId);
    }
  }

  bool UILayer::HasElement(const std::string &elementId) const
  {
    return _elements.find(elementId) != _elements.end();
  }
} // namespace Base
