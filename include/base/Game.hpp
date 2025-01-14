#pragma once
#include <functional>
#include <memory>
namespace Base
{

  class Scene;
  class Game
  {
  public:
    Game();
    ~Game();
    Game(Game &game) = delete;
    Game &operator=(Game &game) = delete;

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
} // namespace Base
