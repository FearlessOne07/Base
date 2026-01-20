#include "base/rendering/RenderingManager.hpp"
#include "base/rendering/RenderContextSingleton.hpp"
#include "base/rendering/RenderLayer.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Colors.hpp"
#include "base/util/Ref.hpp"
#include "internal/rendering/Renderer.hpp"
#include "internal/scene/SceneManager.hpp"
#include <algorithm>
#include <memory>
#include <ranges>

namespace Base
{
  void RenderingManager::SetSceneManager(Ref<SceneManager> sceneManager)
  {
    _sceneManager = sceneManager;
  }

  Ref<RenderLayer> RenderingManager::InitLayer(                                                   //
    const std::weak_ptr<const Scene> ownerScene, Vector2 position, Vector2 size, Color clearColor //
  )
  {
    if (_renderLayers.contains(ownerScene.lock()->GetSceneID()))
    {
      _renderLayers.at(ownerScene.lock()->GetSceneID()).emplace_back(_sceneManager, position, size, clearColor);
      return _renderLayers.at(ownerScene.lock()->GetSceneID()).back();
    }
    return Ref<RenderLayer>();
  }

  void RenderingManager::SetCurrentScene(SceneID scene)
  {
    _currentScene = scene;

    if (!_renderLayers.contains(_currentScene))
    {
      _renderLayers[_currentScene];
    }
  }

  void RenderingManager::RemoveSceneLayers(SceneID scene)
  {
    if (_renderLayers.contains(scene))
    {
      _renderLayers.erase(scene);
    }
  }

  void RenderingManager::Init(const GameConfig &spec)
  {
    Renderer::Init({
      .Title = spec.Title,
      .MinWindowSize = spec.MinWindowSize,
      .Vysnc = spec.Vsync,
      .ResizableWindow = spec.ResizableWindow,
    });

    _renderResolution = spec.Resolution;

    _renderTexture = FrameBuffer::Create({.Width = spec.Resolution.x, .Height = spec.Resolution.y});
    _ping = FrameBuffer::Create({.Width = spec.Resolution.x, .Height = spec.Resolution.y});

    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<ScenePoppedSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto popped = std::dynamic_pointer_cast<ScenePoppedSignal>(signal))
      {
        this->RemoveSceneLayers(popped->Scene);
      }
    });

    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto pushed = std::dynamic_pointer_cast<ScenePushedSignal>(signal))
      {
        this->SetCurrentScene(pushed->Scene);
      }
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> signal) {
      if (auto pushed = std::dynamic_pointer_cast<SceneResumedSignal>(signal))
      {
        this->SetCurrentScene(pushed->Scene);
      }
    });
  }

  void RenderingManager::DeInit()
  {
    _renderLayers.clear();
    FrameBuffer::Destroy(_renderTexture);
    FrameBuffer::Destroy(_ping);
  }

  void RenderingManager::Update(float dt)
  {
    Renderer::Update();
    auto &layers = _renderLayers.at(_currentScene);
    for (auto &layer : layers)
    {
      layer.Update(dt);
    }
  }

  void RenderingManager::Render()
  {

    Renderer::BeginFrame();

    // Begin rendering of Scenes
    auto &layers = _renderLayers.at(_currentScene);
    for (auto &layer : layers)
    {
      layer.Render();
    }

    // Composite Layers
    Renderer::BeginFramebuffer(_renderTexture);
    Renderer::Clear(_sceneManager->GetCurrentScene()->GetClearColor());
    auto layersRev = std::views::reverse(_renderLayers.at(_currentScene));
    for (auto &layer : layers)
    {
      auto rd = RenderContextSingleton::GetInstance();
      Renderer::DrawFramebuffer( //
        layer.GetFramebuffer(), {rd->gameWidth, rd->gameHeight},
        FramebufferAttachmentIndex::Color0 //
      );
    }
    Renderer::EndFramebuffer();

    const auto &scenePost = _sceneManager->GetCurrentScene()->GetPostProcessingEffects();
    Ptr<FrameBuffer> input = _renderTexture;
    Ptr<FrameBuffer> output = _ping;

    for (auto &effect : scenePost)
    {
      effect->Apply(input, output, _renderResolution);
      std::swap(input, output); // Ping-pong
    }

    if (input != _renderTexture)
    {
      Renderer::BeginFramebuffer(_renderTexture);
      Renderer::DrawFramebuffer(input, _renderResolution, FramebufferAttachmentIndex::Color0);
      Renderer::EndFramebuffer();
    }

    auto rd = RenderContextSingleton::GetInstance();

    // Draw Render texture to the Screen
    Renderer::Clear(Black);
    Renderer::DrawFramebuffer( //
      _renderTexture, {rd->marginX, rd->marginY}, Vector2{rd->gameWidth, rd->gameHeight} * rd->scale,
      FramebufferAttachmentIndex::Color0 //
    );

    Renderer::EndFrame();
  }
} // namespace Base
