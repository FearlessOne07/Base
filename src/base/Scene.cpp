#include "base/Scene.hpp"
#include <memory>

namespace Base
{
  struct Scene::SceneState
  {
    SceneTransition sceneTransition = SceneTransition();
    Color fillColor = BLACK;
    EntityManager *entityManager = nullptr;
  };

  void Scene::SetSceneClearColor(Color color)
  {
    _state->fillColor = color;
  }

  void Scene::SetSceneTransition(SceneTransition transition)
  {
    _state->sceneTransition = transition;
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
    _state->sceneTransition = {SceneRequest::NONE, -1};
  }

  void Scene::SetEntityManager(EntityManager *manager)
  {
    if (manager)
    {
      _state->entityManager = manager;
    }
  }

  Scene::Scene() : _state(std::make_unique<SceneState>())
  {
  }
  Scene::~Scene()
  {
  }
} // namespace Base
