#include "base/renderer/Renderer.hpp"
#include "base/renderer/RenderContext.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/renderer/RenderLayer.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "raylib.h"
#include <memory>
#include <ranges>

namespace Base
{
  RenderLayer *Renderer::InitLayer(                                                               //
    const Scene *ownerScene, Vector2 position, Vector2 size, std::function<void()> renderFunction //
  )
  {
    if (_renderLayers.contains(ownerScene))
    {
      _renderLayers.at(ownerScene).emplace_back(ownerScene, position, size, renderFunction);
      return &_renderLayers.at(ownerScene).back();
    }
    return nullptr;
  }

  void Renderer::SetCurrentScene(const Scene *scene)
  {
    _currentScene = scene;

    if (!_renderLayers.contains(_currentScene))
    {
      _renderLayers[_currentScene];
    }
  }

  void Renderer::RemoveSceneLayers(const Scene *scene)
  {
    if (_renderLayers.contains(scene))
    {
      _renderLayers.erase(scene);
    }
  }

  void Renderer::Init(int width, int height)
  {
    _renderTexture = LoadRenderTexture(width, height);
    _shaderBuffer.ping = LoadRenderTexture(width, height);
    _shaderBuffer.pong = LoadRenderTexture(width, height);

    auto bus = SignalBus::GetInstance();

    bus->SubscribeSignal<ScenePoppedSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto popped = std::dynamic_pointer_cast<ScenePoppedSignal>(signal))
      {
        this->RemoveSceneLayers(popped->scene);
      }
    });

    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto pushed = std::dynamic_pointer_cast<ScenePushedSignal>(signal))
      {
        this->SetCurrentScene(pushed->scene);
      }
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto pushed = std::dynamic_pointer_cast<SceneResumedSignal>(signal))
      {
        this->SetCurrentScene(pushed->scene);
      }
    });
  }

  void Renderer::DeInit()
  {
    _renderLayers.clear();
    UnloadRenderTexture(_renderTexture);
    UnloadRenderTexture(_shaderBuffer.ping);
    UnloadRenderTexture(_shaderBuffer.pong);
  }

  void Renderer::RenderLayers()
  {
    // Begin rendering of Scenes
    auto &layers = _renderLayers.at(_currentScene);
    for (auto &layer : layers)
    {
      layer.Render();
    }
  }

  void Renderer::CompositeLayers()
  {
    BeginTextureMode(_renderTexture);
    ClearBackground(_currentScene->GetClearColor());

    auto layers = std::ranges::reverse_view(_renderLayers.at(_currentScene));
    for (auto &layer : layers)
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
      _renderTexture.texture, {0, 0, rd->gameWidth, rd->gameHeight},
      {(float)rd->marginX, (float)rd->marginY, rd->gameWidth * rd->scale, rd->gameHeight * rd->scale}, {0, 0}, 0.f,
      WHITE //
    );
    EndDrawing();
  }
} // namespace Base
