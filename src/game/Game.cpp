#include "base/game/Game.hpp"
#include "base/game/GameConfig.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/InputEvent.hpp"
#include "base/renderer/RenderContext.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/System.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include "internal/game/GameImpl.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace Base
{
  void Game::GameImpl::Init(GameConfig config)
  {
    // Init Audio
    _audioMan.Init();
    _audioMan.SetAssetManager(&_assetManager);

    // Initialize Raylib
    if (config.ResizableWindow)
    {
      SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    }
    if (config.Vsync)
    {
      SetConfigFlags(FLAG_VSYNC_HINT);
    }
    InitWindow(config.MinWindowSize.x, config.MinWindowSize.y, config.Title);
    SetExitKey(0);
    SetWindowMinSize(1280, 720);
    if (config.TargetFps > 0)
    {
      SetTargetFPS(config.TargetFps);
    }

    // Init Renderer
    _renderer.Init(config.Resolution.x, config.Resolution.y);

    // Initialise Render Texture
    _gameWidth = static_cast<float>(config.Resolution.x);
    _gameHeight = static_cast<float>(config.Resolution.y);
    _running = true;

    // Init Systems
    _particleManager.Init();
    _sceneManager.SetQuitCallBack([this]() { this->Quit(); });

    // Initialise InputManager
    _inpMan.Init();
    _inpMan.RegisterListener(this);

    // Initialise Shader Manager
    _shaderManager.Init();

    // Load Global Assets
    _assetManager.Init();
    if (config.GlobalAssets.size() > 0)
    {
      for (auto &[type, path] : config.GlobalAssets)
      {
        std::string assetType = Strings::ToLower(type);

        if (assetType == "font")
        {
          _assetManager.LoadAsset<BaseFont>(path);
        }
        else if (assetType == "sound")
        {
          _assetManager.LoadAsset<Sound>(path);
        }
        else if (assetType == "audio-stream")
        {
          _assetManager.LoadAsset<AudioStream>(path);
        }
        else if (assetType == "shader")
        {
          _assetManager.LoadAsset<BaseShader>(path);
        }
        else if (assetType == "texture")
        {
          _assetManager.LoadAsset<Texture>(path);
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Invalid Asset type speicfied in global assets");
        }
      }
    }

    // Initialize Systems
    _systemManager.Init();

    // Initialize render context
    auto windowWidth = static_cast<float>(GetScreenWidth());
    auto windowHeight = static_cast<float>(GetScreenHeight());
    float scale = std::min( //
      (float)windowWidth / _gameWidth,
      (float)windowHeight / _gameHeight //
    );
    float marginX = (windowWidth - (_gameWidth * scale)) / 2;
    float marginY = (windowHeight - (_gameHeight * scale)) / 2;

    RenderContext rendercontext = {
      .gameWidth = _gameWidth,
      .gameHeight = _gameHeight,
      .marginX = (float)marginX,
      .marginY = (float)marginY,
      .scale = scale,
      .mousePosition =
        {
          (GetMousePosition().x - marginX) / scale,
          (GetMousePosition().y - marginY) / scale,
        },
    };
    RenderContextSingleton::UpdateInstance(&rendercontext);
  }

  void Game::GameImpl::Run()
  {
    // Loop Wule the window is Open
    while (!WindowShouldClose() && _running)
    {
      if (!IsWindowMinimized())
      {
        float windowWidth = 0;
        float windowHeight = 0;
        if (_fullscreen)
        {
          int monitor = GetCurrentMonitor();
          windowWidth = static_cast<float>(GetMonitorWidth(monitor));
          windowHeight = static_cast<float>(GetMonitorHeight(monitor));
        }
        else
        {
          if (Vector2LengthSqr(_lastScreenSize) != 0)
          {
            windowWidth = _lastScreenSize.x;
            windowHeight = _lastScreenSize.y;
            _lastScreenSize = {0, 0};
          }
          else
          {
            windowWidth = static_cast<float>(GetScreenWidth());
            windowHeight = static_cast<float>(GetScreenHeight());
          }
        }
        std::cout << "Window Size: " << windowWidth << ", " << windowHeight << "\n";
        float scale = std::min(             //
          (float)windowWidth / _gameWidth,  //
          (float)windowHeight / _gameHeight //
        );
        float marginX = (windowWidth - (_gameWidth * scale)) / 2;
        float marginY = (windowHeight - (_gameHeight * scale)) / 2;
        const RenderContext *rd = RenderContextSingleton::GetInstance();
        RenderContext rendercontext = {
          .gameWidth = _gameWidth,
          .gameHeight = _gameHeight,
          .marginX = (float)marginX,
          .marginY = (float)marginY,
          .scale = scale,
          .mousePosition = {(GetMousePosition().x - marginX) / scale, (GetMousePosition().y - marginY) / scale},
        };
        RenderContextSingleton::UpdateInstance(&rendercontext);

        // Delta Time
        float dt = GetFrameTime();

        // Update Managers
        _inpMan.PollAndDispatch();

        _sceneManager.Update(dt);

        _uiManager.Update(dt);

        _renderer.Update(dt);

        _systemManager.Update(dt);

        _particleManager.Update(dt);

        _tweenManager.Update(dt);

        _shaderManager.Update(dt);

        // Post Update
        _entityManager.RemoveDeadEntities();
        _inpMan.PostUpdate();

        // Render
        _renderer.RenderLayers();
        _renderer.CompositeLayers();
        _renderer.Render();
      }
    }

    // Cleanup
    End();
  }

  void Game::GameImpl::End()
  {
    // Deinitilize Systems
    _audioMan.DeInit();
    _renderer.DeInit();
    _assetManager.Deinit();
    CloseWindow();
  }

  void Game::GameImpl::Quit()
  {
    _running = false;
  }

  void Game::GameImpl::RegisterScene(std::type_index sceneID, FactoryCallBack factory, bool startScene)
  {
    _sceneManager.RegisterScene(sceneID, std::move(factory), startScene);
  }

  void Game::GameImpl::RegisterSystem(                                            //
    std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem //
  )
  {
    // Register a custom system
    _systemManager.RegisterSystem(systemID, std::move(system), isRenderSystem);
  }

  void Game::GameImpl::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
    if (auto keyEvent = std::dynamic_pointer_cast<KeyEvent>(event))
    {
      if (keyEvent->key == KEY_F11 && keyEvent->action == InputEvent::Action::PRESSED)
      {
        if (!_fullscreen)
        {
          _lastScreenSize = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
        }
        ToggleFullscreen();
        _fullscreen = !_fullscreen;
        event->isHandled = true;
        return;
      }
    }
    _sceneManager.OnInputEvent(event);
  }

  // Game Class
  Game::Game()
  {
    _impl = new GameImpl;
  }

  Game::~Game()
  {
    delete _impl;
  }

  void Game::Run()
  {
    _impl->Run();
  }

  void Game::Init(GameConfig config)
  {
    _impl->Init(config);
  }

  void Game::RegisterSceneImpl(                                                               //
    std::type_index sceneID, std::function<std::unique_ptr<Scene>()> factory, bool startScene //
  )
  {
    _impl->RegisterScene(sceneID, std::move(factory), startScene);
  }

  void Game::RegisterSystemImpl(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem)
  {
    _impl->RegisterSystem(systemID, std::move(system), isRenderSystem);
  }
} // namespace Base
