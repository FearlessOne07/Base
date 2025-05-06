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

  void Scene::SetParticleManager(ParticleManager *manager)
  {
    if (manager)
    {
      _state->particleManager = manager;
    }
  }

  void Scene::SetCameraManager(CameraManager *manager)
  {
    if (manager)
    {
      _state->cameraManager = manager;
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

  Scene::Scene() : _layerStack(this), _state(std::make_unique<SceneState>())
  {
  }

  void Scene::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
    _layerStack.OnInputEvent(event);
  }
} // namespace Base
