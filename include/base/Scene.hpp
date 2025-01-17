#pragma once
#include "base/Exports.hpp"
#include "base/SceneData.hpp"
#include "base/SceneTransition.hpp"
#include "raylib.h"
#include <memory>
#include <typeindex>
namespace Base
{
  class EntityManager;
  class BASEAPI Scene
  {
  private:
    friend class SceneManager; // SceneManager
    SceneTransition GetSceneTransition() const;
    void SetEntityManager(EntityManager *manager);
    void ResetSceneTransition();
    void __setSceneTransition(std::type_index sceneID, SceneRequest request, SceneData data = SceneData());

    struct SceneState;
    std::unique_ptr<SceneState> _state;

  protected: // Inclass Access
    void SetSceneClearColor(Color color);
    void Clear() const;

    template <typename T = void> void SetSceneTransition(SceneRequest request, SceneData data = SceneData())
    {
      __setSceneTransition(typeid(T), request, data);
    }

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
