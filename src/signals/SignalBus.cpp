#include "base/signals/SignalBus.hpp"
#include <future>
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

    std::future<void> future = std::async(std::launch::async, [this, event]() {
      auto handlerId = std::type_index(typeid(*event));
      auto it = _handlers.find(handlerId);

      if (it != _handlers.end())
      {
        for (const SignalHandler &handler : it->second)
        {
          handler(event); // called synchronously inside the async block
        }
      }
    });
  }
} // namespace Base
