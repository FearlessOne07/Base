#include "base/game/Game.hpp"
#include "base/assets/BaseAsset.hpp"
#include "base/game/GameConfig.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/InputEvent.hpp"
#include "base/input/Keys.hpp"
#include "base/rendering/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/System.hpp"
#include "base/util/Exception.hpp"
#include "internal/game/GameImpl.hpp"
#include "internal/rendering/Renderer.hpp"
#include <algorithm>
#include <memory>
#include <utility>

// Hint dGPU
#ifdef _WIN32
#include <windows.h>

extern "C"
{
  _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
  _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif
// TODO: Maybe make a base Manager class for _currentScene functionality
namespace Base
{
  void Game::GameImpl::Init(const GameConfig &config)
  {
    // Init Audio
    _audioMan.Init();
    _audioMan.SetAssetManager(&_assetManager);

    _renderingManager.Init(config);

    // Initialise Render Texture
    _gameWidth = static_cast<float>(config.Resolution.x);
    _gameHeight = static_cast<float>(config.Resolution.y);
    _running = true;

    // Init Systems
    _sceneManager.SetQuitCallBack([this]() { this->Quit(); });
    _uiManager.Init();
    _particleManager.Init(_sceneManager);

    // Initialise InputManager
    _inpMan.Init();
    _inpMan.RegisterListener(*this);

    // Initialise Shader Manager

    _renderingManager.SetSceneManager(_sceneManager);
    _systemManager.SetSceneManager(_sceneManager);

    // Load Global Assets
    _assetManager.Init();
    if (config.GlobalAssets.size() > 0)
    {
      for (const auto &[type, pathList] : config.GlobalAssets)
      {

        switch (type)
        {
        case AssetType::Texture:
          for (const auto &path : pathList)
          {
            _assetManager.LoadTexture(path, true);
          }
          break;
        case AssetType::Sound:
          for (const auto &path : pathList)
          {
            _assetManager.LoadSound(path, true);
          }
          break;
        case AssetType::AudioStream:
          for (const auto &path : pathList)
          {
            _assetManager.LoadAudioStream(path, true);
          }
          break;
        case AssetType::Font:
          for (const auto &path : pathList)
          {
            _assetManager.LoadFont(path, true);
          }
          break;
        case AssetType::Shader:
          for (const auto &path : pathList)
          {
            _assetManager.LoadShader(path, true);
          }
          break;
        default:
          THROW_BASE_RUNTIME_ERROR("Invalid Asset type speicfied in global assets");
        };
      }
    }

    // Initialize Systems
    _systemManager.Init();

    // Tween Manaher
    _tweenManager.Init();

    // Initialize render context
    auto windowWidth = static_cast<float>(Renderer::GetWindowSize().x);
    auto windowHeight = static_cast<float>(Renderer::GetWindowSize().y);
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
          (Renderer::GetWindowMousePosition().x - marginX) / scale,
          (Renderer::GetWindowMousePosition().y - marginY) / scale,
        },
    };
    RenderContextSingleton::UpdateInstance(&rendercontext);
  }

  void Game::GameImpl::Run()
  {
    // Loop Wule the window is Open
    while (!Renderer::IsWindowClosed() && _running)
    {
      // if (!IsWindowMinimized())
      // {
      auto windowWidth = static_cast<float>(Renderer::GetWindowSize().x);
      auto windowHeight = static_cast<float>(Renderer::GetWindowSize().y);

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
        .mousePosition = {(Renderer::GetWindowMousePosition().x - marginX) / scale,
                          (Renderer::GetWindowMousePosition().y - marginY) / scale},
      };
      RenderContextSingleton::UpdateInstance(&rendercontext);

      // Delta Time
      float dt = 1.f / 60.f;

      // Update Managers
      _inpMan.PollAndDispatch();

      _sceneManager.Update(dt);

      _uiManager.Update(dt);

      _renderingManager.Update(dt);

      _systemManager.Update(dt);

      _particleManager.Update(dt);

      _tweenManager.Update(dt);

      // Render
      _renderingManager.RenderLayers();
      // _renderengManager.CompositeLayers();
      // _renderengManager.Render();

      // Post Update
      _inpMan.PostUpdate();
      _entityManager.RemoveDeadEntities();
      _sceneManager.PostUpdate();
      // }
      // else
      // {
      // }
    }

    // Cleanup
    End();
  }

  void Game::GameImpl::End()
  {
    // Deinitilize Systems
    _audioMan.DeInit();
    _assetManager.Deinit();
    _renderingManager.DeInit();
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
      if (keyEvent->Key == Key::F11 && keyEvent->action == InputEvent::Action::Pressed)
      {
        event->isHandled = true;
        return;
      }
    }
    _sceneManager.OnInputEvent(event);
  }

  void Game::GameImpl::ToggleFullscreenBorderless()
  {
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
    std::type_index sceneID, std::function<std::shared_ptr<Scene>()> factory, bool startScene //
  )
  {
    _impl->RegisterScene(sceneID, std::move(factory), startScene);
  }

  void Game::RegisterSystemImpl(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem)
  {
    _impl->RegisterSystem(systemID, std::move(system), isRenderSystem);
  }
} // namespace Base
