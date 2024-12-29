#pragma once
#include "base/Game.hpp"
#include "internal/scene/SceneManager.hpp"
#include "raylib.h"
#include <functional>
#include <memory>
namespace Base
{

class Scene;
class Game::GameImpl
{
  // Type defs
  using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

private:
  bool _running;
  RenderTexture _renderTexture;
  float _gameWidth;
  float _gameHeight;

private: // Systems
  SceneManager _scenemanager;

private: // Methods
  void Quit();
  void End();

public:
  GameImpl() = default;
  void Init(int width, int height, const char *title, int fps = 0);
  void RegisterScene(int sceneID, FactoryCallBack factory);
  void Run();
};
} // namespace Base
