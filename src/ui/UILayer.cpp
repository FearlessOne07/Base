#include "base/ui/UILayer.hpp"
namespace Base
{

  void UILayer::Add(const std::string &elementId, std::shared_ptr<UIElement> &element)
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
      throw std::runtime_error("Specified UI element does not exist");
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
      throw std::runtime_error("Specified UI element does not exist");
    }
    else
    {
      return _elements.at(elementId);
    }
  }
} // namespace Base
