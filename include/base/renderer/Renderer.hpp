#pragma once

#include "base/renderer/RenderLayer.hpp"
#include "raylib.h"
#include <deque>
#include <unordered_map>

namespace Base
{
  class Scene;
  class Renderer
  {
    friend class Game;

  private:
    std::unordered_map<const Scene *, std::deque<RenderLayer>> _renderLayers;
    RenderTexture2D _renderTexture;
    const Scene *_currentScene = nullptr;
    Vector2 _renderResolution;

    // Shaders
    RenderTexture2D _ping;

  private:
    void Init(int width, int height);
    void DeInit();
    void Update(float dt);
    void RenderLayers();
    void CompositeLayers();
    void Render();

    void SetCurrentScene(const Scene *scene);
    void RemoveSceneLayers(const Scene *scene);

  public:
    RenderLayer *InitLayer(                                                     //
      const Scene *ownerScene, Vector2 position, Vector2 size, Color clearColor //
    );
    void SetClearColor();
  };
} // namespace Base
