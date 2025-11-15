#pragma once
#include "Signal.hpp"
#include "base/scenes/SceneID.hpp"
#include <functional>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Base
{
  class Scene;
  class SignalBus
  {
    using SignalHandler = std::function<void(const std::shared_ptr<Signal> &)>;

  private:
    static SignalBus *_instance;

  private:
    std::unordered_map<SceneID, std::unordered_map<std::type_index, std::vector<SignalHandler>>> _handlers = {};
    SceneID _currentScene;

  public:
    static SignalBus *GetInstance();

    template <typename T>
      requires(std::is_base_of_v<Signal, T>)
    void SubscribeSignal(const SignalHandler &handler)
    {
      // Get the event ID
      auto id = std::type_index(typeid(T));

      if (!_handlers.contains(_currentScene))
      {
        _handlers[_currentScene];
      }

      // Check if a vector of handlers for that event exists
      if (_handlers.at(_currentScene).find(id) == _handlers.at(_currentScene).end())
      {
        // If not create one
        _handlers.at(_currentScene)[id] = {};
      }

      // Append tha handler to the list of handers for that event
      _handlers.at(_currentScene).at(id).emplace_back(std::move(handler));
    }

    void BroadCastSignal(const std::shared_ptr<Signal> &);
  };
} // namespace Base
