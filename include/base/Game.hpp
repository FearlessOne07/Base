#pragma once
#include <functional>
#include <memory>

class Scene;
class Game
{
public:
  Game() = default;
  Game(Game &game) = delete;
  Game &operator=(Game &g) = delete;

  template <typename T> void RegisterScene(int sceneID)
  {
    RegisterSceneImpl(sceneID, []() -> std::unique_ptr<Scene> { return std::make_unique<T>(); });
  };
  void Init(int width, int height, const char *title, int fps = 0);
  void Run();

private:
  class GameImpl;
  GameImpl *_impl = nullptr;
  void RegisterSceneImpl(int sceneID, std::function<std::unique_ptr<Scene>()> factory);
};
