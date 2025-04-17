#pragma once

#include "base/Event.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
namespace Base
{
  class InputManager
  {
    std::unordered_map<int, int> _heldKeys;
    std::unordered_map<int, int> _heldMouseBtns;

    std::vector<int> _handledKeyPresses;
    std::vector<int> _handledMousePresses;

    std::shared_ptr<Event> _lastEvent = nullptr;

  public:
    void PollAndDispatch();
    void PostUpdate();
  };
} // namespace Base
