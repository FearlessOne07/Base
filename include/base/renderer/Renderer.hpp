#pragma once

#include "base/renderer/RenderLayer.hpp"
#include <functional>
#include <vector>
namespace Base
{
  class Renderer
  {
    friend class Game;

  private:
    std::vector<RenderLayer> _renderLayers;
    RenderTexture2D _renderTexture;

  private:
    void Init(int width, int height);
    void DeInit();
    void RenderLayers();
    void CompositeLayers();

    void Render();

  public:
    void InitLayer(Vector2 position, Vector2 size, std::function<void()> renderFunction);
    void SetClearColor();
  };
} // namespace Base
