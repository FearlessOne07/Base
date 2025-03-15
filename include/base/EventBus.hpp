#pragma once
#include "base/Event.hpp"
#include "base/Exports.hpp"
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Base
{
  class BASEAPI EventBus
  {
    using EventHandler = std::function<void(const Event &)>;

  private:
    std::unordered_map<std::type_index, std::vector<EventHandler>> _handlers = {};

  public:
    template <typename T> void SubscribeEvent(EventHandler &handler)
    {
      if (!std::is_base_of_v<Event, T>)
      {
        throw std::runtime_error("T must be a derivative of the  Event class");
      }
      auto id = std::type_index(typeid(T));
      if (_handlers.find(id) == _handlers.end())
      {
        _handlers[id] = {};
      }
      _handlers[id].emplace_back(std::move(handler));
    }

    void Dispatch(const Event &);
  };
} // namespace Base
