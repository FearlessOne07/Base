#include "internal/input/InputManager.hpp"
#include "base/EventBus.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "raylib/raylib.h"
#include <algorithm>
#include <memory>

namespace Base
{
  void InputManager::PollAndDispatch()
  {
    auto *bus = Base::EventBus::GetInstance();

    // Track key state
    for (int key = KEY_NULL; key <= KEY_KB_MENU; ++key)
    {
      if (IsKeyDown(key))
      {
        if (!_heldKeys.contains(key) && std::ranges::find(_handledKeyPresses, key) == _handledKeyPresses.end())
        {
          _heldKeys[key] = 1;
          std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
          event->key = key;
          event->action = KeyEvent::Action::PRESSED;
          bus->Dispatch(event);

          if (event->isHandled)
          {
            _handledKeyPresses.push_back(key);
          }
        }
        else
        {
          if (std::ranges::find(_handledKeyPresses, key) == _handledKeyPresses.end())
          {
            ++_heldKeys.at(key);
            std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
            event->key = key;
            event->action = KeyEvent::Action::HELD;
            bus->Dispatch(event);
          }
        }
      }
      else if (IsKeyReleased(key))
      {
        _heldKeys.erase(key);
        auto it = std::ranges::find(_handledKeyPresses, key);

        if (it != _handledKeyPresses.end())
        {
          _handledKeyPresses.erase(it);
        }

        std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
        event->key = key;
        event->action = KeyEvent::Action::RELEASED;
        bus->Dispatch(event);
      }
    }

    // Mouse
    for (int btn = 0; btn <= MOUSE_BUTTON_RIGHT; ++btn)
    {
      if (IsMouseButtonDown(btn))
      {
        if (!_heldMouseBtns.contains(btn) && std::ranges::find(_handledMousePresses, btn) == _handledMousePresses.end())
        {
          _heldMouseBtns[btn] = 1;
          std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
          event->button = btn;
          event->action = MouseButtonEvent::Action::PRESSED;
          bus->Dispatch(event);

          if (event->isHandled)
          {
            _handledMousePresses.push_back(btn);
          }
        }
        else
        {
          if (std::ranges::find(_handledMousePresses, btn) == _handledMousePresses.end())
          {
            ++_heldMouseBtns.at(btn);
            std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
            event->button = btn;
            event->action = MouseButtonEvent::Action::HELD;
            bus->Dispatch(event);
          }
        }
      }
      else if (IsMouseButtonReleased(btn))
      {
        _heldMouseBtns.erase(btn);

        auto it = std::ranges::find(_handledMousePresses, btn);

        if (it != _handledMousePresses.end())
        {
          _handledMousePresses.erase(it);
        }

        std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
        event->button = btn;
        event->action = MouseButtonEvent::Action::RELEASED;
        bus->Dispatch(event);
      }
    }
  }
} // namespace Base
