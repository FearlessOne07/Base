#pragma once

#include "base/renderer/RenderLayer.hpp"
#include "base/renderer/ShaderBuffer.hpp"
#include "raylib.h"
#include <functional>
#include <unordered_map>
#include <vector>
namespace Base
{
  class Scene;
  class Renderer
  {
    friend class Game;

  private:
    std::unordered_map<const Scene *, std::vector<RenderLayer>> _renderLayers;
    RenderTexture2D _renderTexture;
    const Scene *_currentScene = nullptr;

    // Shaders
    ShaderBuffer _shaderBuffer;

  private:
    void Init(int width, int height);
    void DeInit();
    void RenderLayers();
    void CompositeLayers();
    void Render();

    void SetCurrentScene(const Scene *scene);
    void RemoveSceneLayers(const Scene *scene);

  public:
    RenderLayer *InitLayer(                                                                         //
      const Scene *ownerScene, Vector2 position, Vector2 size, std::function<void()> renderFunction //
    );
    void SetClearColor();
  };
} // namespace Base
