#include "base/signals/SignalBus.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include <memory>
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
    if (auto it = std::dynamic_pointer_cast<ScenePushedSignal>(event))
    {
      _currentScene = it->scene;
    }
    else if (auto it = std::dynamic_pointer_cast<ScenePoppedSignal>(event))
    {
      if (_handlers.contains(it->scene))
      {
        _handlers.erase(it->scene);
      }
    }

    if (!_handlers.contains(_currentScene))
    {
      _handlers[_currentScene];
    }

    auto handlerId = std::type_index(typeid(*(event)));
    auto it = _handlers.at(_currentScene).find(handlerId);
    if (it != _handlers.at(_currentScene).end())
    {
      for (SignalHandler &handler : _handlers.at(_currentScene).at(handlerId))
      {
        handler(event);
      }
    }

    it = _handlers.at(0).find(handlerId);
    if (it != _handlers.at(0).end())
    {
      for (SignalHandler &handler : _handlers.at(0).at(handlerId))
      {
        handler(event);
      }
    }
  }
} // namespace Base
