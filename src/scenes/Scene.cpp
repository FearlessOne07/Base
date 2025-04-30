#include "base/scenes/Scene.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/scenes/SceneData.hpp"
#include <memory>

namespace Base
{

  void Scene::SetClearColor(Color color)
  {
    _state->fillColor = color;
  }

  void Scene::_setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data)
  {
    _state->sceneTransition = {
      .request = request,
      .sceneID = sceneID,
      .data = data,
    };
  }

  void Scene::Clear() const
  {
    ClearBackground(_state->fillColor);
  };

  SceneTransition Scene::GetSceneTransition() const
  {
    return _state->sceneTransition;
  }
  void Scene::ResetSceneTransition()
  {
    _state->sceneTransition = {.request = SceneRequest::NONE, .sceneID = typeid(-1)};
  }
  SceneLayerStack Scene::GetLayerStack()
  {
    return _layerStack;
  }

  void Scene::SetEntityManager(EntityManager *manager)
  {
    if (manager)
    {
      _state->entityManager = manager;
    }
  }

  void Scene::SetParticleManager(ParticleManager *manager)
  {
    if (manager)
    {
      _state->particleManager = manager;
    }
  }

  EntityManager *Scene::GetEntityManager() const
  {
    return _state->entityManager;
  }

  ParticleManager *Scene::GetParticleManager() const
  {
    return _state->particleManager;
  }

  Scene::Scene() : _state(std::make_unique<SceneState>())
  {
  }

  void Scene::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    _layerStack.OnInputEvent(event);
  }
} // namespace Base
