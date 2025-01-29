#pragma once
#include "base/Exports.hpp"
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <typeindex>

namespace Base
{
  class Scene;
  class System;
  class BASEAPI Game
  {
  public:
    Game();
    ~Game();
    Game(Game &game) = delete;
    Game &operator=(Game &game) = delete;

    template <typename T> void RegisterScene()
    {
      if (std::is_base_of<Scene, T>())
      {
        std::type_index sceneID = std::type_index(typeid(T));
        RegisterSceneImpl(sceneID, []() -> std::unique_ptr<Scene> { return std::make_unique<T>(); });
      }
      else
      {
        throw std::runtime_error("Type Must be a derivative if the Scene class");
      }
    };

    template <typename T> void RegisterSystem()
    {
      if (std::is_base_of<System, T>())
      {
        std::type_index systemID = std::type_index(typeid(T));
        RegisterSystemImpl(systemID, std::make_unique<T>());
      }
      else
      {
        throw std::runtime_error("Type Must be a derivative if the Scene class");
      }
    };
    void Init(int width, int height, const char *title, int fps = 0);
    void Run();

  private:
    class GameImpl;
    GameImpl *_impl = nullptr;
    void RegisterSceneImpl(std::type_index sceneID, std::function<std::unique_ptr<Scene>()> factory);
    void RegisterSystemImpl(std::type_index systemID, std::unique_ptr<System> system);
  };
} // namespace Base
