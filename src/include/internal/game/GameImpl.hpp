#pragma once
#include "base/AssetManager.hpp"
#include "base/EntityManager.hpp"
#include "base/Game.hpp"
#include "base/SystemManager.hpp"
#include "base/ui/UIManager.hpp"
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
    UIManager _uiManager = UIManager();
    EntityManager _entityManager = EntityManager();
    AssetManager _assetManager = AssetManager();
    SystemManager _systemmanager = SystemManager(&_entityManager);
    SceneManager _scenemanager = SceneManager(&_uiManager, &_entityManager, &_systemmanager, &_assetManager);

  private: // Methods
    void Quit();
    void End();
    void OnKeyEvent(const std::shared_ptr<KeyEvent> &event);

  public:
    GameImpl() = default;
    void Init(int width, int height, const char *title, int fps = 0);
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory);
    void RegisterSystem(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem);
    void Run();
  };
} // namespace Base
