#pragma once
#include "base/assets/AssetManager.hpp"
#include "base/camera/CameraManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/game/Game.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/renderer/Renderer.hpp"
#include "base/systems/SystemManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
#include "internal/audio/AudioManager.hpp"
#include "internal/input/InputManager.hpp"
#include "internal/scene/SceneManager.hpp"
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
    float _gameWidth = 0.f;
    float _gameHeight = 0.f;

  private: // Systems
    AudioManager _audioMan = AudioManager();
    Renderer _renderer = Renderer();
    InputManager _inpMan = InputManager();
    EntityManager _entityManager = EntityManager();
    AssetManager _assetManager = AssetManager();
    CameraManager _cameraManager = CameraManager();
    ParticleManager _particleManager = ParticleManager();
    UIManager _uiManager = UIManager();
    TweenManager _tweenManager = TweenManager();
    SystemManager _systemManager = SystemManager(&_entityManager);
    SceneManager _sceneManager = SceneManager( //
      &_renderer, &_entityManager, &_systemManager, &_assetManager, &_particleManager, &_cameraManager, &_uiManager,
      &_tweenManager //
    );

  private: // Methods
    void Quit();
    void End();
    void UpdateRenderContext();

  public:
    GameImpl() = default;
    void Init(int width, int height, const char *title, int fps = 0);
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory, bool startScene);
    void RegisterSystem(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem);
    void Run();
  };
} // namespace Base
