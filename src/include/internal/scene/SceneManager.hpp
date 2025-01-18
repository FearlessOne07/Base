#pragma once
#include "base/Exports.hpp"
#include "base/Scene.hpp"
#include <functional>
#include <memory>
#include <stack>

namespace Base
{
  class EntityManager;
  class BASEAPI SceneManager
  {
    // Type Defs
    using QuitCallBack = std::function<void()>;
    using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

  private:
    QuitCallBack _quitCallBack = nullptr;
    std::unordered_map<int, FactoryCallBack> _factories;
    EntityManager *_entityManager = nullptr;

  private:
    std::stack<std::shared_ptr<Scene>> _scenes;

    void PushScene(int sceneID, SceneData sceneData = SceneData());
    void ReplaceScene(int sceneId, SceneData sceneData = SceneData());
    void PopScene();

  public:
    SceneManager(EntityManager *entityManager);
    SceneManager() = default;
    void RegisterScene(int sceneID, FactoryCallBack factory);
    void Update(float dt);
    void Render();

    void SetQuitCallBack(QuitCallBack quitCallback);
  };
} // namespace Base
