#include "base/EventBus.hpp"

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

  void EventBus::Dispatch(const Event &event)
  {
    auto it = _handlers.find(std::type_index(typeid(event)));

    if (it != _handlers.end())
    {
      for (EventHandler &handler : _handlers[std::type_index(typeid(event))])
      {
        handler(event);
      }
    }
  }
} // namespace Base
