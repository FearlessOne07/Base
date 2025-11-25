#pragma once

#include "base/renderer/RenderLayer.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/util/Ref.hpp"
#include "raylib.h"
#include <deque>
#include <unordered_map>

namespace Base
{
  class SceneID;
  class Renderer
  {
    friend class Game;

  private:
    std::unordered_map<SceneID, std::deque<RenderLayer>> _renderLayers;
    RenderTexture2D _renderTexture;
    SceneID _currentScene;
    Vector2 _renderResolution;

    // Shaders
    RenderTexture2D _ping;

    Ref<ShaderManager> _shaderManager;
    Ref<SceneManager> _sceneManager;

  private:
    Renderer(Ref<ShaderManager> shaderManager);
    void SetSceneManager(Ref<SceneManager> sceneManager);
    void Init(int width, int height);
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
