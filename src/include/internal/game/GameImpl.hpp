#pragma once
#include "base/AssetManager.hpp"
#include "base/EntityManager.hpp"
#include "base/Game.hpp"
#include "base/SystemManager.hpp"
#include "internal/scene/SceneManager.hpp"
#include "raylib/raylib.h"
#include <functional>
#include <memory>

namespace Base
{

  class Scene;
  class System;
  class   Game::GameImpl
  {
    // Type defs
    using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

  private:
    bool _running = false;
    RenderTexture _renderTexture;
    float _gameWidth = 0.f;
    float _gameHeight = 0.f;

  private: // Systems
    EntityManager _entityManager = EntityManager();
    SystemManager _systemmanager = SystemManager(&_entityManager);
    AssetManager _assetManager = AssetManager();
    SceneManager _scenemanager = SceneManager(&_entityManager, &_systemmanager, &_assetManager);

  private: // Methods
    void Quit();
    void End();

  public:
    GameImpl() = default;
    void Init(int width, int height, const char *title, int fps = 0);
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory);
    void RegisterSystem(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem);
    void Run();
  };
} // namespace Base
