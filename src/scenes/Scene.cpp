#include "base/scenes/Scene.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/scenes/SceneData.hpp"
#include <memory>

namespace Base
{

  void Scene::_setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data)
  {
    _state->sceneTransition = {
      .request = request,
      .sceneID = sceneID,
      .data = data,
    };
  }

  SceneTransition Scene::GetSceneTransition() const
  {
    return _state->sceneTransition;
  }
  void Scene::ResetSceneTransition()
  {
    _state->sceneTransition = {.request = SceneRequest::NONE, .sceneID = typeid(-1)};
  }
  SceneLayerStack &Scene::GetLayerStack()
  {
    return _layerStack;
  }

  void Scene::SetSystemManager(SystemManager *manager)
  {
    if (manager)
    {
      _state->systemManager = manager;
    }
  }

  void Scene::SetAssetManager(AssetManager *manager)
  {
    if (manager)
    {
      _state->assetManager = manager;
    }
  }

  void Scene::SetUIManager(UIManager *manager)
  {
    if (manager)
    {
      _state->uiManager = manager;
    }
  }

  void Scene::SetParticleManager(ParticleManager *manager)
  {
    if (manager)
    {
      _state->particleManager = manager;
    }
  }
  void Scene::SetEntityManager(EntityManager *manager)
  {
    if (manager)
    {
      _state->entityManager = manager;
    }
  }

  void Scene::SetCameraManager(CameraManager *manager)
  {
    if (manager)
    {
      _state->cameraManager = manager;
    }
  }

  void Scene::SetTweenManager(TweenManager *manager)
  {
    if (manager)
    {
      _state->tweenManager = manager;
    }
  }

  EntityManager *Scene::GetEntityManager() const
  {
    return _state->entityManager;
  }

  CameraManager *Scene::GetCameraManager() const
  {
    return _state->cameraManager;
  }

  SystemManager *Scene::GetSystemManager() const
  {
    return _state->systemManager;
  }

  AssetManager *Scene::GetAssetManager() const
  {
    return _state->assetManager;
  }

  ParticleManager *Scene::GetParticleManager() const
  {
    return _state->particleManager;
  }

  UIManager *Scene::GetUIManager() const
  {
    return _state->uiManager;
  }

  TweenManager *Scene::GetTweenManager() const
  {
    return _state->tweenManager;
  }

  Scene::Scene() : _layerStack(this), _state(std::make_unique<SceneState>())
  {
  }

  void Scene::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
    _layerStack.OnInputEvent(event);
  }

  void Scene::Update(float dt)
  {
    _layerStack.Update(dt);
  }

  void Scene::Render()
  {
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
} // namespace Base
