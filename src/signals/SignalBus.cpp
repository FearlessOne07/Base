#include "base/signals/SignalBus.hpp"
#include <typeindex>

namespace Base
{
  SignalBus *SignalBus::_instance = nullptr;
  SignalBus *SignalBus::GetInstance()
  {
    if (!_instance)
    {
      _instance = new SignalBus;
    }
    return _instance;
  }

  void SignalBus::BroadCastSignal(const std::shared_ptr<Signal> &event)
  {
    auto handlerId = std::type_index(typeid(*(event)));
    auto it = _handlers.find(handlerId);

    if (it != _handlers.end())
    {
      for (SignalHandler &handler : _handlers[handlerId])
      {
        handler(event);
      }
    }
  }
} // namespace Base
