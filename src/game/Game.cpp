#include "base/game/Game.hpp"
#include "base/game/RenderContext.hpp"
#include "base/game/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/System.hpp"
#include "internal/game/GameImpl.hpp"
#include "internal/input/InputManager.hpp"
#include "raylib.h"
#include <algorithm>
#include <memory>
#include <utility>

namespace Base
{
  void Game::GameImpl::Init(int width, int height, const char *title, int fps)
  {
    // Initialize Raylib
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, title);
    SetExitKey(0);
    SetWindowMinSize(1280, 720);
    if (fps > 0)
    {
      SetTargetFPS(fps);
    }

    // Initialise Render Texture
    _renderTexture = LoadRenderTexture(width, height);
    _gameWidth = static_cast<float>(_renderTexture.texture.width);
    _gameHeight = static_cast<float>(_renderTexture.texture.height);
    _running = true;

    // Init Systems
    _particleManager.Init();
    _scenemanager.SetQuitCallBack([this]() { this->Quit(); });

    _inpMan.RegisterListener(&_scenemanager);

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
        .camera = rd->camera,
      };
      RenderContextSingleton::UpdateInstance(&rendercontext);

      // Delta Time
      float dt = GetFrameTime();

      _inpMan.PollAndDispatch();

      _scenemanager.Update(dt);

      _cameraManager.Update(dt);

      _systemmanager.Update(dt);

      _particleManager.Update(dt);

      // Post Update
      _entityManager.RemoveDeadEntities();
      _inpMan.PostUpdate();

      // Begin rendering of Scenes
      BeginTextureMode(_renderTexture);
      ClearBackground(BLACK);
      _scenemanager.Render();
      EndTextureMode();

      // Draw Render texture to the Screen
      BeginDrawing();
      ClearBackground(BLACK);
      DrawTexturePro( //
        _renderTexture.texture, {0, 0, _gameWidth, -_gameHeight},
        {(float)marginX, (float)marginY, _gameWidth * scale, _gameHeight * scale}, {0, 0}, 0.f, WHITE //
      );
      EndDrawing();
    }

    // Cleanup
    End();
  }

  void Game::GameImpl::End()
  {
    UnloadRenderTexture(_renderTexture);
    CloseWindow();
  }

  void Game::GameImpl::Quit()
  {
    _running = false;
  }

  void Game::GameImpl::RegisterScene(std::type_index sceneID, FactoryCallBack factory)
  {
    _scenemanager.RegisterScene(sceneID, std::move(factory));
  }

  void Game::GameImpl::RegisterSystem(                                                    //
    std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem = false //
  )
  {
    _systemmanager.RegisterSystem(systemID, std::move(system), isRenderSystem);
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

  void Game::Init(int width, int height, const char *title, int fps)
  {
    _impl->Init(width, height, title, fps);
  }

  void Game::RegisterSceneImpl(std::type_index sceneID, std::function<std::unique_ptr<Scene>()> factory)
  {
    _impl->RegisterScene(sceneID, std::move(factory));
  }

  void Game::RegisterSystemImpl(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem = false)
  {
    _impl->RegisterSystem(systemID, std::move(system), isRenderSystem);
  }
} // namespace Base
