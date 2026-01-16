#pragma once

#include "base/input/InputEvent.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "base/ui/UILayer.hpp"
#include "base/util/Ref.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace Base
{
  class SceneID;
  class UIManager
  {
    std::unordered_map<SceneID, std::unordered_map<std::string, UILayer>> _layers;
    SceneID _currentScene;

    void UnloadSceneUI(SceneID scene);

  public:
    void Init();
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    Ref<UILayer> AddLayer( //
      const std::string &layerID, Vector2 layerSize, Vector2 layerPosition,
      ConstRef<SceneLayer> parentLayer //
    );
    void RenderLayer(Ref<UILayer> layer);
    void Update(float dt);
    void UpdateCurrentScene(SceneID scene);
  };
} // namespace Base
