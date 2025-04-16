#include "internal/input/InputManager.hpp"
#include "base/EventBus.hpp"
#include "base/input/Events/KeyEvent.hpp"
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
        if (!_heldKeys.contains(key))
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
        _handledKeyPresses.erase(std::ranges::find(_handledKeyPresses, key));

        std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
        event->key = key;
        event->action = KeyEvent::Action::RELEASED;
        bus->Dispatch(event);
      }
    }

    // // Mouse
    // for (int btn = 0; btn <= MOUSE_BUTTON_RIGHT; ++btn)
    // {
    //   if (IsMouseButtonDown(btn))
    //   {
    //   }
    //   else if (IsMouseButtonReleased(btn))
    //   {
    //   }
    // }
  }
} // namespace Base
