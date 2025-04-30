#pragma once
#include "Signal.hpp"
#include "base/util/Exception.hpp"
#include <functional>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Base
{
  class SignalManager
  {
    using SignalHandler = std::function<void(const std::shared_ptr<Signal> &)>;

  private:
    static SignalManager *_instance;

  private:
    std::unordered_map<std::type_index, std::vector<SignalHandler>> _handlers = {};

  public:
    static SignalManager *GetInstance();

    template <typename T> void SubscribeSignal(const SignalHandler &handler)
    {
      // Check if T is a derivative of Base::Signal
      if (!std::is_base_of_v<Signal, T>)
      {
        THROW_BASE_RUNTIME_ERROR("T must be a derivative of the  Signal class");
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

    void BroadCastSignal(const std::shared_ptr<Signal> &);
  };
} // namespace Base
