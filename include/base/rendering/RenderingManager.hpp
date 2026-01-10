#pragma once

#include "base/rendering/RenderLayer.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/util/Ref.hpp"
#include <deque>
#include <unordered_map>

namespace Base
{
  class SceneID;
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

    Ref<ShaderManager> _shaderManager;
    Ref<SceneManager> _sceneManager;

  private:
    RenderingManager(Ref<ShaderManager> shaderManager);
    void SetSceneManager(Ref<SceneManager> sceneManager);
    void Init(const RenderSpec &);
    void DeInit();
    void Update(float dt);
    void RenderLayers();
    void CompositeLayers();
    void Render();

    void SetCurrentScene(SceneID scene);
    void RemoveSceneLayers(SceneID scene);

  public:
    Ref<RenderLayer> InitLayer(                                                                     //
      const std::weak_ptr<const Scene> ownerScene, Vector2 position, Vector2 size, Color clearColor //
    );
  };
} // namespace Base
