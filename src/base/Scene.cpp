#include "base/Scene.hpp"
#include "base/SceneData.hpp"
#include <memory>

namespace Base
{
  struct Scene::SceneState
  {
    SceneTransition sceneTransition = SceneTransition();
    Color fillColor = BLACK;
    EntityManager *entityManager = nullptr;
  };

  void Scene::SetClearColor(Color color)
  {
    _state->fillColor = color;
  }

  void Scene::__setSceneTransition(std::type_index sceneID, SceneRequest request, SceneData data)
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
    _state->sceneTransition = {SceneRequest::NONE, typeid(-1)};
  }

  void Scene::SetEntityManager(EntityManager *manager)
  {
    if (manager)
    {
      _state->entityManager = manager;
    }
  }

  EntityManager *Scene::GetEntityManager() const
  {
    return _state->entityManager;
  }

  Scene::Scene() : _state(std::make_unique<SceneState>())
  {
  }

  Scene::~Scene()
  {
  }
} // namespace Base
