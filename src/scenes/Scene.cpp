#include "base/scenes/Scene.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/renderer/Renderer.hpp"
#include "base/scenes/SceneData.hpp"
#include "base/scenes/signals/SceneLayerPausedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/systems/SystemManager.hpp"
#include <memory>

namespace Base
{

  SceneID::operator bool()
  {
    return _id >= 0;
  }

  SceneID::operator int64_t() const
  {
    return _id;
  }

  bool SceneID::operator==(const SceneID &other)
  {
    return _id == other._id;
  }

  SceneID::SceneID(int64_t id)
  {
    _id = id;
  }

  SceneID::SceneID()
  {
    _id = -1;
  }

  void Scene::_setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data)
  {
    _state->sceneTransition = {
      .request = request,
      .sceneID = sceneID,
      .data = data,
    };
  }

  void Scene::SetSceneID(SceneID id)
  {
    _state->sceneID = id;
  }

  SceneID Scene::GetSceneID() const
  {
    return _state->sceneID;
  }

  const SceneTransition &Scene::GetSceneTransition() const
  {
    return _state->sceneTransition;
  }
  void Scene::ResetSceneTransition()
  {
    _state->sceneTransition = {.request = SceneRequest::None, .sceneID = typeid(-1)};
  }

  void Scene::SetRenderer(Ref<Renderer> manager)
  {
    if (manager)
    {
      _state->renderer = manager;
    }
  }

  void Scene::SetShaderManager(Ref<ShaderManager> manager)
  {
    if (manager)
    {
      _state->shaderManager = manager;
    }
  }

  void Scene::SetSystemManager(Ref<SystemManager> manager)
  {
    if (manager)
    {
      _state->systemManager = manager;
    }
  }

  void Scene::SetAssetManager(Ref<AssetManager> manager)
  {
    if (manager)
    {
      _state->assetManager = manager;
    }
  }

  void Scene::SetUIManager(Ref<UIManager> manager)
  {
    if (manager)
    {
      _state->uiManager = manager;
    }
  }

  void Scene::SetParticleManager(Ref<ParticleManager> manager)
  {
    if (manager)
    {
      _state->particleManager = manager;
    }
  }

  void Scene::SetEntityManager(Ref<EntityManager> manager)
  {
    if (manager)
    {
      _state->entityManager = manager;
    }
  }

  void Scene::SetTweenManager(Ref<TweenManager> manager)
  {
    if (manager)
    {
      _state->tweenManager = manager;
    }
  }

  void Scene::SetClearColor(Color color)
  {
    _state->clearColor = color;
  }

  Color Scene::GetClearColor() const
  {
    return _state->clearColor;
  }

  Ref<Renderer> Scene::GetRenderer() const
  {
    return _state->renderer;
  }

  Ref<ShaderManager> Scene::GetShaderManager() const
  {
    return _state->shaderManager;
  }

  Ref<EntityManager> Scene::GetEntityManager() const
  {
    return _state->entityManager;
  }

  Ref<SystemManager> Scene::GetSystemManager() const
  {
    return _state->systemManager;
  }

  Ref<AssetManager> Scene::GetAssetManager() const
  {
    return _state->assetManager;
  }

  Ref<ParticleManager> Scene::GetParticleManager() const
  {
    return _state->particleManager;
  }

  Ref<UIManager> Scene::GetUIManager() const
  {
    return _state->uiManager;
  }

  Ref<TweenManager> Scene::GetTweenManager() const
  {
    return _state->tweenManager;
  }

  void Scene::Init()
  {
    _state = std::make_unique<SceneState>();
    _layerStack = SceneLayerStack(shared_from_this());
  }

  void Scene::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
  }

  void Scene::_OnInputEvent(std::shared_ptr<InputEvent> event)
  {
    OnInputEvent(event);
    _layerStack.OnInputEvent(event);
  }

  void Scene::Update(float dt)
  {
    _layerStack.Update(dt);
  }

  void Scene::Render()
  {
    ClearBackground(_state->clearColor);
    _layerStack.Render();
  }

  void Scene::Resume()
  {
  }

  void Scene::Suspend()
  {
  }

  // Temmplate Methods
  void Scene::_exit()
  {
    _layerStack.DetachLayers();
    Exit();
  }

  Ref<RenderLayer> Scene::AddRenderLayer(Vector2 size, Color clearColor)
  {
    return GetRenderer()->InitLayer(shared_from_this(), {0, 0}, size, clearColor);
  }

  void Scene::SuspendSystems()
  {
    GetSystemManager()->Suspend();
  }

  void Scene::UnsuspendSystems()
  {
    GetSystemManager()->Unsuspend();
  }

  void Scene::StartSystems()
  {
    GetSystemManager()->StartSystems();
  }

  void Scene::StopSystems()
  {
    GetSystemManager()->StopSystems();
  }

  void Scene::PauseLayer(int layerIndex)
  {
    auto bus = SignalBus::GetInstance();
    bus->BroadCastSignal(std::make_shared<SceneLayerPausedSignal>());
    _pauseMask.set(layerIndex);
  }

  void Scene::UnPauseLayer(int layerIndex)
  {
    _pauseMask.reset(layerIndex);
  }

  bool Scene::IsLayerPaused(int layerIndex)
  {
    return _pauseMask.test(layerIndex);
  }

  const std::bitset<8> &Scene::GetPauseMask() const
  {
    return _pauseMask;
  }

  const ShaderEffectChain &Scene::GetPostProcessingEffects() const
  {
    return _postProcessingEffects;
  }

} // namespace Base
