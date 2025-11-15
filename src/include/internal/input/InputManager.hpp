#pragma once
#include "base/input/InputEvent.hpp"
#include "base/signals/Signal.hpp"
#include "base/util/Ref.hpp"
#include "internal/input/InputListener.hpp"
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

    std::shared_ptr<InputEvent> _lastEvent = nullptr;
    std::vector<Ref<InputListener>> _listenrs;

  private:
    void ResetInput(const std::shared_ptr<Signal> &sig);

  public:
    void Init();
    void PollAndDispatch();
    void PostUpdate();
    void RegisterListener(InputListener &listener);
    void DispatchEvent(std::shared_ptr<InputEvent> event);
  };
} // namespace Base
