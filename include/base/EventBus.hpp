#pragma once
#include "base/Exports.hpp"
#include <cstddef>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Base
{
  class BASEAPI EventBus
  {
    using EventHandler = std::function<void(size_t entityID)>;

  private:
    std::unordered_map<std::type_index, std::vector<EventHandler>> _handlers = {};

  public:
    template <typename T> void SubscribeEvent(EventHandler handler)
    {
      auto id = std::type_index(typeid(T));
      if (_handlers.find(id) == _handlers.end())
      {
        _handlers[id] = {};
      }
      _handlers[id].emplace_back(std::move(handler));
    }
  };
} // namespace Base
