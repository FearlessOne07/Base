#include "base/signals/SignalBus.hpp"
#include "base/util/Exception.hpp"
#include <future>
#include <iostream>
#include <ostream>
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
      try
      {
        auto handlerId = std::type_index(typeid(*event));
        auto it = _handlers.find(handlerId);

        if (it != _handlers.end())
        {
          for (const SignalHandler &handler : it->second)
          {
            handler(event); // might throw
          }
        }
      }
      catch (BaseException &e)
      {
        std::cout << e.what();
        std::abort();
      }
    });
  }
} // namespace Base
