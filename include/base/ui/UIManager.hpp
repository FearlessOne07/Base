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

  public:
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    UILayer *AddLayer(const std::string &layerID, Vector2 layerSize);
    UILayer *GetLayer(const std::string &layerID);
    void RemoveLayer(const std::string &layerID);
    void RenderLayer(const std::string &layerId);
    void Update(float dt);
  };
} // namespace Base
