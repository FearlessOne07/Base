#include "base/game/Game.hpp"
#include "base/game/GameConfig.hpp"
#include "base/renderer/RenderContext.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/System.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include "internal/game/GameImpl.hpp"
#include "raylib.h"
#include <algorithm>
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
    _inpMan.Init();
    _particleManager.Init();
    _sceneManager.SetQuitCallBack([this]() { this->Quit(); });
    _inpMan.RegisterListener(&_sceneManager);

    // Load Global Assets
    _assetManager.Init();

    _shaderManager.Init();

    if (config.globalAssets.size() > 0)
    {
      for (auto &[type, path] : config.globalAssets)
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
    while (!WindowShouldClose() && _running)
    {
      // Update render context
      auto windowWidth = static_cast<float>(GetScreenWidth());
      auto windowHeight = static_cast<float>(GetScreenHeight());
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

      _inpMan.PollAndDispatch();

      _sceneManager.Update(dt);

      _uiManager.Update(dt);

      _cameraManager.Update(dt);

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

    // Cleanup
    End();
  }

  void Game::GameImpl::End()
  {
    _assetManager.Deinit();
    _audioMan.DeInit();
    _renderer.DeInit();
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
    _systemManager.RegisterSystem(systemID, std::move(system), isRenderSystem);
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
