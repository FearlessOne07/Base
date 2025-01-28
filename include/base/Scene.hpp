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
  class SystemManager;
  class AssetManager;
  class BASEAPI Scene
  {
  private:
    friend class SceneManager; // SceneManager
    SceneTransition GetSceneTransition() const;
    void SetEntityManager(EntityManager *);
    void ResetSceneTransition();
    void __setSceneTransition(std::type_index sceneID, SceneRequest request, SceneData data = SceneData());

    struct SceneState;
    std::unique_ptr<SceneState> _state;

  protected: // Inclass Access
    void Clear() const;
    void SetSceneClearColor(Color color);
    EntityManager *GetEntityManager() const;

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
    virtual void Enter(SystemManager *systemManager, AssetManager *assetManager, SceneData sceneData = SceneData()) = 0;
    virtual void Render() = 0;
    virtual void Exit(SystemManager *systemManager, AssetManager *assetManager) = 0;
  };
} // namespace Base
