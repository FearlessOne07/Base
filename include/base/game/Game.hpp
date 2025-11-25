#pragma once
#include "base/game/GameConfig.hpp"
#include <functional>
#include <memory>
#include <type_traits>
#include <typeindex>

namespace Base
{
  class Scene;
  class System;
  class Game
  {
  public:
    Game();
    ~Game();
    Game(Game &game) = delete;
    Game &operator=(Game &game) = delete;

    template <typename T>
      requires(std::is_base_of_v<Scene, T>)
    void RegisterScene(bool startScene = false)
    {
      auto sceneID = std::type_index(typeid(T));
      RegisterSceneImpl( //
        sceneID, std::move([]() -> std::shared_ptr<Scene> { return std::make_shared<T>(); }),
        startScene //
      );
    };

    template <typename T>
      requires(std::is_base_of_v<System, T>)
    void RegisterSystem(bool isRenderSystem = false)
    {
      auto systemID = std::type_index(typeid(T));
      RegisterSystemImpl(systemID, std::move(std::make_unique<T>()), isRenderSystem);
    };
    void Init(GameConfig gameConig = GameConfig());
    void Run();

  private:
    class GameImpl;

    // Pointer To Game Implementation
    GameImpl *_impl = nullptr;
    void RegisterSceneImpl(std::type_index sceneID, std::function<std::shared_ptr<Scene>()> factory, bool startScene);
    void RegisterSystemImpl(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem);
  };
} // namespace Base
