#pragma once
#include "base/input/InputEvent.hpp"
#include "base/input/Keys.hpp"
#include "base/input/MouseButtons.hpp"
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
    std::unordered_map<Key, int> _heldKeys;
    std::unordered_map<MouseKey, int> _heldMouseBtns;

    std::vector<Key> _handledKeyPresses;
    std::vector<MouseKey> _handledMousePresses;

    std::shared_ptr<InputEvent> _lastEvent = nullptr;
    std::vector<Ref<InputListener>> _listenrs;

  private:
    void ResetInput(const std::shared_ptr<Signal> &sig);
    void DispatchEvent(std::shared_ptr<InputEvent> event);

  public:
    void Init();
    void DispatchKeyEvent(int key, int scancode, int action, int mods);
    void DispatchMouseEvent(int button, int action, int mods);
    void PostUpdate();
    void RegisterListener(InputListener &listener);
  };
} // namespace Base
