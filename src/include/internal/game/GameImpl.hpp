#pragma once
#include "base/assets/AssetManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/game/Game.hpp"
#include "base/game/GameConfig.hpp"
#include "base/input/InputEvent.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/renderer/Renderer.hpp"
#include "base/shaders/ShaderManager.hpp"
#include "base/systems/SystemManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
#include "internal/audio/AudioManager.hpp"
#include "internal/input/InputListener.hpp"
#include "internal/input/InputManager.hpp"
#include "internal/scene/SceneManager.hpp"
#include <functional>
#include <memory>

namespace Base
{

  class Scene;
  class System;
  class Game::GameImpl : InputListener
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
    ShaderManager _shaderManager = ShaderManager(_assetManager);
    Renderer _renderer = Renderer(_shaderManager);
    SystemManager _systemManager = SystemManager(_entityManager);

    SceneManager _sceneManager = SceneManager({
      _renderer,
      _entityManager,
      _systemManager,
      _assetManager,
      _particleManager,
      _uiManager,
      _tweenManager,
      _shaderManager,
    });

  private: // Methods
    void Quit();
    void End();
    void UpdateRenderContext();
    void ToggleFullscreenBorderless();

  public:
    GameImpl() = default;
    void Init(GameConfig config);
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory, bool startScene);
    void RegisterSystem(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem);
    void OnInputEvent(std::shared_ptr<InputEvent> event) override;
    void Run();
  };
} // namespace Base
