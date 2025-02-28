#pragma once
#include "base/Exports.hpp"
#include "base/SceneData.hpp"
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
  class BASEAPI SceneManager
  {
    // Type Defs
    using QuitCallBack = std::function<void()>;
    using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

  private:
    QuitCallBack _quitCallBack = nullptr;
    std::unordered_map<std::type_index, FactoryCallBack> _factories;
    EntityManager *_entityManager = nullptr;
    SystemManager *_systemManager = nullptr;
    AssetManager *_assetManager = nullptr;

  private:
    std::stack<std::shared_ptr<Scene>> _scenes;

    void PushScene(std::type_index sceneID, const SceneData &sceneData = SceneData());
    void ReplaceScene(std::type_index sceneId, const SceneData &sceneData = SceneData());
    void PopScene();

  public:
    SceneManager(EntityManager *entityManager, SystemManager *systemManager, AssetManager *assetManager);
    SceneManager() = default;
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory);
    void Update(float dt);
    void Render();

    void SetQuitCallBack(QuitCallBack quitCallback);
  };
} // namespace Base
