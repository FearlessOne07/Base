#pragma once

#include "base/input/InputEvent.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "base/ui/UILayer.hpp"
#include "base/util/Ref.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace Base
{
  class Scene;
  class UIManager
  {
    std::unordered_map<const Scene *, std::unordered_map<std::string, UILayer>> _layers;
    const Scene *_currentScene;

    void UnloadSceneUI(const Scene *scene);

  public:
    void Init();
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    Ref<UILayer> AddLayer( //
      const std::string &layerID, Vector2 layerSize, Vector2 layerPosition,
      const SceneLayer &parentLayer //
    );
    void RenderLayer(const std::string &layerId);
    void Update(float dt);
    void UpdateCurrentScene(const Scene *scene);
  };
} // namespace Base
