#pragma once
#include "base/SceneTransition.hpp"
#include "raylib.h"
#include <memory>
namespace Base
{
  class EntityManager;
  class Scene
  {
  private:
    friend class SceneManager; // SceneManager
    void SetEntityManager(EntityManager *manager);
    SceneTransition GetSceneTransition() const;
    void ResetSceneTransition();

    struct SceneState;
    std::unique_ptr<SceneState> _state;

  protected: // Inclass Access
    void SetSceneClearColor(Color color);
    void SetSceneTransition(SceneTransition transition);
    void Clear() const;

  protected: // Virtual
    virtual void GetInput() = 0;

  public:
    Scene();
    virtual ~Scene();
    virtual void Update(float dt) = 0;
    virtual void Enter(SceneData sceneData = SceneData()) = 0;
    virtual void Render() = 0;
    virtual void Exit() = 0;
  };
} // namespace Base
