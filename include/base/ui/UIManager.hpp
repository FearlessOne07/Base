#pragma once

#include "base/input/InputEvent.hpp"
#include "base/ui/UILayer.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace Base
{
  class Scene;
  class UIManager
  {
    std::unordered_map<const Scene *, std::unordered_map<std::string, UILayer>> _layers;
    const Scene *_currentScene = nullptr;

    void UnloadSceneUI(const Scene *scene);

  public:
    void Init();
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    UILayer *AddLayer(const std::string &layerID, Vector2 layerSize, Vector2 layerPosition = {0, 0});
    void RenderLayer(const std::string &layerId);
    void Update(float dt);
    void UpdateCurrentScene(const Scene *scene);
  };
} // namespace Base
