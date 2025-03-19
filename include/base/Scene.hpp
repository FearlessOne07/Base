#pragma once
#include "base/SceneData.hpp"
#include "base/SceneTransition.hpp"
#include "raylib/raylib.h"
#include <memory>
#include <typeindex>

namespace Base
{
  class EntityManager;
  class SystemManager;
  class AssetManager;
  class   Scene
  {
  private:
    friend class SceneManager; // SceneManager
    [[nodiscard]] SceneTransition GetSceneTransition() const;
    void SetEntityManager(EntityManager *);
    void ResetSceneTransition();
    void _setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data = SceneData());

    struct SceneState
    {
      SceneTransition sceneTransition = SceneTransition();
      Color fillColor = BLACK;
      EntityManager *entityManager = nullptr;
    };

    std::unique_ptr<SceneState> _state;

  protected: // Inclass Access
    void Clear() const;
    void SetClearColor(Color color);
    [[nodiscard]] EntityManager *GetEntityManager() const;

    template <typename T = void> void SetSceneTransition(SceneRequest request, const SceneData &data = SceneData())
    {
      _setSceneTransition(typeid(T), request, data);
    }

  protected: // Virtual
    virtual void GetInput() = 0;

  public:
    Scene();
    virtual ~Scene() = default;
    virtual void Update(float dt, SystemManager *systemManager) = 0;
    virtual void Enter(SystemManager *systemManager, AssetManager *assetManager, SceneData sceneData = SceneData()) = 0;
    virtual void Render(Base::SystemManager *systemManager) = 0;
    virtual void Exit(SystemManager *systemManager, AssetManager *assetManager) = 0;
  };
} // namespace Base
