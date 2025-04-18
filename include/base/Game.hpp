#pragma once
#include <functional>
#include <memory>
#include <stdexcept>
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

    template <typename T> void RegisterScene()
    {
      // Check if T is a derivative of Base::Scene
      if (std::is_base_of<Scene, T>())
      {
        auto sceneID = std::type_index(typeid(T));
        RegisterSceneImpl(sceneID, std::move([]() -> std::unique_ptr<Scene> { return std::make_unique<T>(); }));
      }
      else
      {
        throw std::runtime_error("Type Must be a derivative if the Scene class");
      }
    };

    template <typename T> void RegisterSystem(bool isRenderSystem = false)
    {
      // Check if T is a derivative of Base::System
      if (std::is_base_of<System, T>())
      {
        auto systemID = std::type_index(typeid(T));
        RegisterSystemImpl(systemID, std::move(std::make_unique<T>()), isRenderSystem);
      }
      else
      {
        throw std::runtime_error("Type Must be a derivative if the System class");
      }
    };
    void Init(int width, int height, const char *title, int fps = 0);
    void Run();

  private:
    class GameImpl;

    // Pointer To Game Implementation
    GameImpl *_impl = nullptr;
    void RegisterSceneImpl(std::type_index sceneID, std::function<std::unique_ptr<Scene>()> factory);
    void RegisterSystemImpl(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem);
  };
} // namespace Base
