#pragma once
#include "base/EntityManager.hpp"
#include "base/Exports.hpp"
#include "base/Game.hpp"
#include "base/SystemManager.hpp"
#include "internal/scene/SceneManager.hpp"
#include "raylib.h"
#include <functional>
#include <memory>

namespace Base
{

  class Scene;
  class System;
  class BASEAPI Game::GameImpl
  {
    // Type defs
    using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

  private:
    bool _running;
    RenderTexture _renderTexture;
    float _gameWidth;
    float _gameHeight;

  private: // Systems
    EntityManager _entityManager = EntityManager();
    SystemManager _systemmanager = SystemManager(&_entityManager);
    SceneManager _scenemanager = SceneManager(&_entityManager, &_systemmanager);

  private: // Methods
    void Quit();
    void End();

  public:
    GameImpl() = default;
    void Init(int width, int height, const char *title, int fps = 0);
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory);
    void RegisterSystem(std::type_index systemID, std::unique_ptr<System> system);
    void Run();
  };
} // namespace Base
