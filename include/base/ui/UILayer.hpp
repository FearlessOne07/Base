#pragma once
#include "base/ui/UIElement.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace Base
{
  class UILayer
  {
    friend class UIManager;

  private:
    std::unordered_map<std::string, std::shared_ptr<UIElement>> _elements;

  private:
    void Add(const std::string &elementId, const std::shared_ptr<UIElement> &element);
    void Remove(const std::string &elementId);
    std::shared_ptr<UIElement> &Get(const std::string &elementID);
  };
} // namespace Base
