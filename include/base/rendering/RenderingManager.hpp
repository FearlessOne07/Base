#pragma once

#include "base/game/GameConfig.hpp"
#include "base/rendering/RenderLayer.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/util/Ref.hpp"
#include <deque>
#include <unordered_map>

namespace Base
{
  class RenderSpec;
  class RenderingManager
  {
    friend class Game;

  private:
    Ptr<FrameBuffer> _renderTexture;
    std::unordered_map<SceneID, std::deque<RenderLayer>> _renderLayers;
    SceneID _currentScene;
    Vector2 _renderResolution;

    // Shaders
    Ptr<FrameBuffer> _ping;
    Ref<SceneManager> _sceneManager;

  private:
    RenderingManager() = default;
    void SetSceneManager(Ref<SceneManager> sceneManager);
    void Init(const GameConfig &);

    void DeInit();
    void Update(float dt);

    void SetCurrentScene(SceneID scene);
    void RemoveSceneLayers(SceneID scene);

  public:
    void Render();
    Ref<RenderLayer> InitLayer(                                                                     //
      const std::weak_ptr<const Scene> ownerScene, Vector2 position, Vector2 size, Color clearColor //
    );
  };
} // namespace Base
