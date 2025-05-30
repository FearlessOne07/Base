#include "base/renderer/Renderer.hpp"
#include "base/renderer/RenderContext.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/renderer/RenderLayer.hpp"
#include "raylib.h"
#include <ranges>

namespace Base
{
  void Renderer::InitLayer(Vector2 position, Vector2 size, std::function<void()> renderFunction)
  {
    _renderLayers.emplace_back(position, size, renderFunction);
  }

  void Renderer::Init(int width, int height)
  {
    _renderTexture = LoadRenderTexture(width, height);
  }

  void Renderer::RenderLayers()
  {
    // Begin rendering of Scenes
    for (auto layer : _renderLayers)
    {
      layer.Render();
    }
  }

  void Renderer::CompositeLayers()
  {
    BeginTextureMode(_renderTexture);
    for (auto &layer : std::ranges::reverse_view(_renderLayers))
    {
      DrawTexturePro( //
        layer.GetTexture()->texture, {0, 0, layer.GetSize().x, layer.GetSize().y},
        {layer.GetPosition().x, layer.GetPosition().y, layer.GetSize().x, layer.GetSize().y}, //
        {0, 0}, 0, WHITE                                                                      //
      );
    }
    EndTextureMode();
  }

  void Renderer::Render()
  {
    auto *rd = RenderContextSingleton::GetInstance();

    // Draw Render texture to the Screen
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro( //
      _renderTexture.texture, {0, 0, rd->gameWidth, -rd->gameHeight},
      {(float)rd->marginX, (float)rd->marginY, rd->gameWidth * rd->scale, rd->gameHeight * rd->scale}, {0, 0}, 0.f,
      WHITE //
    );
    EndDrawing();
  }
} // namespace Base
