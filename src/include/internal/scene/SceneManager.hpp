#pragma once
#include "base/particles/ParticleManager.hpp"
#include "base/scenes/SceneData.hpp"
#include <functional>
#include <memory>
#include <stack>
#include <typeindex>

namespace Base
{
  class Scene;
  class EntityManager;
  class SystemManager;
  class AssetManager;
  class UIManager;
  class SceneManager
  {
    // Type Defs
    using QuitCallBack = std::function<void()>;
    using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

  private:
    QuitCallBack _quitCallBack = nullptr;
    std::unordered_map<std::type_index, FactoryCallBack> _factories;
    UIManager *_uiManager = nullptr;
    EntityManager *_entityManager = nullptr;
    SystemManager *_systemManager = nullptr;
    AssetManager *_assetManager = nullptr;
    ParticleManager *_particleManager = nullptr;

  private:
    std::stack<std::shared_ptr<Scene>> _scenes;

    void PushScene(std::type_index sceneID, const SceneData &sceneData = SceneData());
    void ReplaceScene(std::type_index sceneId, const SceneData &sceneData = SceneData());
    void PopScene();

  public:
    SceneManager(                                                                                                   //
      UIManager *uiManager, EntityManager *entityManager, SystemManager *systemManager, AssetManager *assetManager, //
      ParticleManager *particleManager                                                                              //
    );
    SceneManager() = default;
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory);
    void Update(float dt);
    void Render();

    void SetQuitCallBack(QuitCallBack quitCallback);
  };
} // namespace Base
