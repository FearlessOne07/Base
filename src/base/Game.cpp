#include "base/Game.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "internal/game/GameImpl.hpp"
#include "raylib.h"
#include <algorithm>

namespace Base
{

  void Game::GameImpl::Init(int width, int height, const char *title, int fps)
  {
    // Initialize Raylib
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, title);
    SetExitKey(0);
    SetWindowMinSize(1280, 720);
    if (fps > 0)
    {
      SetTargetFPS(fps);
    }

    // Initialise Render Texture
    _renderTexture = LoadRenderTexture(width, height);
    _gameWidth = _renderTexture.texture.width;
    _gameHeight = _renderTexture.texture.height;
    _running = true;

    // Init Systems
    _scenemanager.SetQuitCallBack([this]() { this->Quit(); });

    // Initialize render context
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    float scale = std::min( //
      (float)windowWidth / _gameWidth,
      (float)windowHeight / _gameHeight //
    );
    int marginX = (windowWidth - (_gameWidth * scale)) / 2;
    int marginY = (windowHeight - (_gameHeight * scale)) / 2;
    RenderContext rendercontext = {
      _gameWidth,     _gameHeight, //
      (float)marginX, (float)marginY, scale,
    };
    RenderContextSingleton::UpdateInstance(&rendercontext);
  }

  void Game::GameImpl::Run()
  {
    while (!WindowShouldClose() && _running)
    {
      // Update render context
      int windowWidth = GetScreenWidth();
      int windowHeight = GetScreenHeight();
      float scale = std::min(             //
        (float)windowWidth / _gameWidth,  //
        (float)windowHeight / _gameHeight //
      );
      int marginX = (windowWidth - (_gameWidth * scale)) / 2;
      int marginY = (windowHeight - (_gameHeight * scale)) / 2;
      RenderContext rendercontext = {
        _gameWidth,     _gameHeight, //
        (float)marginX, (float)marginY, scale,
      };

      RenderContextSingleton::UpdateInstance(&rendercontext);

      // Delta Time
      float dt = GetFrameTime();

      // Update Scene mamnager
      _scenemanager.Update(dt);

      // Begin rendering of Scenes
      BeginTextureMode(_renderTexture);
      _scenemanager.Render();
      EndTextureMode();

      // Draw Render texture to the Screen
      BeginDrawing();
      ClearBackground({33, 34, 39, 255});
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
} // namespace Base
