#pragma once

#include "base/ui/UIElement.hpp"
#include "base/ui/UILayer.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace Base
{
  class UIManager
  {
  private:
    std::unordered_map<std::string, UILayer> _uiLayers = {};

  public:
    void AddElement(const std::string &layerId, const std::string &elementID, std::shared_ptr<UIElement> &element);
    void AddLayer(const std::string &layerId);

    void RemoveElement(const std::string &layerId, const std::string &elementID);
    void RemoveLayer(const std::string &layerId);

    std::shared_ptr<UIElement> &GetElement(const std::string &layerId, const std::string &elementID);

    //
    void RenderLayer(const std::string &layerId);
    void Update();
  };
} // namespace Base
