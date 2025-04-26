#pragma once
#include "base/Event.hpp"
#include <functional>
#include <memory>
#include "internal/Exception/Exception.hpp"
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Base
{
  class EventBus
  {
    using EventHandler = std::function<void(const std::shared_ptr<Event> &)>;

  private:
    static EventBus *_instance;

  private:
    std::unordered_map<std::type_index, std::vector<EventHandler>> _handlers = {};

  public:
    static EventBus *GetInstance();

    template <typename T> void SubscribeEvent(EventHandler handler)
    {
      // Check if T is a derivative of Base::Event
      if (!std::is_base_of_v<Event, T>)
      {
        THROW_BASE_RUNTIME_ERROR("T must be a derivative of the  Event class");
      }

      // Get the event ID
      auto id = std::type_index(typeid(T));

      // Check if a vector of handlers for that event exists
      if (_handlers.find(id) == _handlers.end())
      {
        // If not create one
        _handlers[id] = {};
      }

      // Append tha handler to the list of handers for that event
      _handlers.at(id).emplace_back(std::move(handler));
    }

    void Dispatch(const std::shared_ptr<Event> &);
  };
} // namespace Base
