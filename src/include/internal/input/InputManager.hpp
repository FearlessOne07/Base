#pragma once
#include "base/input/InputEvent.hpp"
#include "base/input/Keys.hpp"
#include "base/signals/Signal.hpp"
#include "base/util/Ref.hpp"
#include "internal/input/InputListener.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace Base
{
  class InputManager
  {
    enum class KeyPosition
    {
      Pressed,
      Down,
      Up
    };

    struct KeyState
    {
      KeyPosition Position = KeyPosition::Up;
      bool Handled = false;
    };

    std::array<KeyState, static_cast<size_t>(Key::Count)> _keyStates;
    std::array<KeyState, static_cast<size_t>(MouseKey::Count)> _mouseKeyStates;

    std::shared_ptr<InputEvent> _lastEvent = nullptr;
    std::vector<Ref<InputListener>> _listenrs;

  private:
    void ResetInput(const std::shared_ptr<Signal> &sig);
    void DispatchEvent(std::shared_ptr<InputEvent> event);

  public:
    void Init();
    void PollAndDispatch();
    void PostUpdate();
    void RegisterListener(InputListener &listener);
  };
} // namespace Base
