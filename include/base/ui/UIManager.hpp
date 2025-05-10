#pragma once

#include "base/input/InputEvent.hpp"
#include "base/ui/UILayer.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace Base
{
  class UIManager
  {
    std::unordered_map<std::string, UILayer> _layers;
    std::shared_ptr<Font> _uiFont;

  public:
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    UILayer &AddLayer(const std::string &layerID);
    UILayer &GetLayer(const std::string &layerID);
    void RemoveLayer(const std::string &layerID);
    void RenderLayer(const std::string &layerId);
    void SetUIFont(std::shared_ptr<Font> &font);
  };
} // namespace Base
