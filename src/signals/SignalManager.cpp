#include "base/signals/SignalManager.hpp"
#include <typeindex>

namespace Base
{
  SignalManager *SignalManager::_instance = nullptr;
  SignalManager *SignalManager::GetInstance()
  {
    if (!_instance)
    {
      _instance = new SignalManager;
    }
    return _instance;
  }

  void SignalManager::BroadCastSignal(const std::shared_ptr<Signal> &event)
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
