#pragma once
#include "base/assets/AssetManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/game/Game.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/systems/SystemManager.hpp"
#include "internal/input/InputManager.hpp"
#include "internal/scene/SceneManager.hpp"
#include "raylib.h"
#include <functional>
#include <memory>

namespace Base
{

  class Scene;
  class System;
  class Game::GameImpl
  {
    // Type defs
    using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

  private:
    bool _running = false;
    RenderTexture _renderTexture;
    float _gameWidth = 0.f;
    float _gameHeight = 0.f;

  private: // Systems
    InputManager _inpMan = InputManager();
    EntityManager _entityManager = EntityManager();
    AssetManager _assetManager = AssetManager();
    ParticleManager _particleManager = ParticleManager();
    SystemManager _systemmanager = SystemManager(&_entityManager);
    SceneManager _scenemanager = SceneManager(                            //
      &_entityManager, &_systemmanager, &_assetManager, &_particleManager //
    );

  private: // Methods
    void Quit();
    void End();
    void UpdateRenderContext();

  public:
    GameImpl() = default;
    void Init(int width, int height, const char *title, int fps = 0);
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory);
    void RegisterSystem(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem);
    void Run();
  };
} // namespace Base
