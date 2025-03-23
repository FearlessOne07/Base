#include "base/EventBus.hpp"
#include <typeindex>

namespace Base
{
  EventBus *EventBus::_instance = nullptr;
  EventBus *EventBus::GetInstance()
  {
    if (!_instance)
    {
      _instance = new EventBus;
    }
    return _instance;
  }

  void EventBus::Dispatch(const std::shared_ptr<Event> &event)
  {
    auto handlerId = std::type_index(typeid(*event));
    auto it = _handlers.find(handlerId);

    if (it != _handlers.end())
    {
      for (EventHandler &handler : _handlers[handlerId])
      {
        handler(event);
      }
    }
  }
} // namespace Base
