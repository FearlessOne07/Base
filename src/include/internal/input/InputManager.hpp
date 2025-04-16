#pragma once

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

  public:
    void PollAndDispatch();
  };
} // namespace Base
