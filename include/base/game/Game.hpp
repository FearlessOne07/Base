#pragma once
#include "base/assets/AssetManager.hpp"
#include "base/audio/AudioManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/game/GameConfig.hpp"
#include "base/input/InputEvent.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/rendering/RenderingManager.hpp"
#include "base/systems/SystemManager.hpp"
#include "base/timing/TimeManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
#include "internal/input/InputListener.hpp"
#include "internal/input/InputManager.hpp"
#include "internal/scene/SceneManager.hpp"
#include <functional>
#include <memory>

namespace Base
{

  class Scene;
  class System;
  class Game : InputListener
  {
    // Type defs
    using FactoryCallBack = std::function<std::shared_ptr<Scene>()>;

  private:
    bool _running = false;
    float _gameWidth = 0.f;
    float _gameHeight = 0.f;
    bool _fullscreen = false;
    Vector2 _lastScreenSize = {0, 0};
    Vector2 _lastScreenPosition{0, 0};

  private: // Systems
    AudioManager _audioMan = AudioManager();
    InputManager _inpMan = InputManager();
    EntityManager _entityManager = EntityManager();
    AssetManager _assetManager = AssetManager();
    ParticleManager _particleManager = ParticleManager();
    UIManager _uiManager = UIManager();
    TweenManager _tweenManager = TweenManager();
    RenderingManager _renderingManager = RenderingManager();
    SystemManager _systemManager = SystemManager(_entityManager);
    TimeManager _timeManager = TimeManager();

    SceneManager _sceneManager = SceneManager(EngineCtx{
      _renderingManager,
      _entityManager,
      _particleManager,
      _assetManager,
      _systemManager,
      _uiManager,
      _tweenManager,
      _timeManager,
    });

  private: // Methods
    void Quit();
    void End();
    void UpdateRenderContext();
    void ToggleFullscreenBorderless();
    void _registerScene(std::type_index sceneID, FactoryCallBack factory, bool startScene);
    void _registerSystem(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem);

  public:
    Game() = default;
    void Init(const GameConfig &config);

    template <typename T>
      requires(std::is_base_of_v<Scene, T>)
    void RegisterScene(bool startScene = false)
    {
      auto sceneID = std::type_index(typeid(T));
      _registerScene( //
        sceneID, std::move([]() -> std::shared_ptr<Scene> { return std::make_shared<T>(); }),
        startScene //
      );
    };

    template <typename T>
      requires(std::is_base_of_v<System, T>)
    void RegisterSystem(bool isRenderSystem = false)
    {
      auto systemID = std::type_index(typeid(T));
      _registerSystem(systemID, std::move(std::make_unique<T>()), isRenderSystem);
    };

    template <typename T> void LoadGlobalAssets()
    {
      _assetManager.InitGlobalAssets<T>();
    }

    void OnInputEvent(std::shared_ptr<InputEvent> event) override;
    void Run();
  };
} // namespace Base
