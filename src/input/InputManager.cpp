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
          _lastEvent = event;

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
            _lastEvent = event;
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
        _lastEvent = event;
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
          _lastEvent = event;

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
            _lastEvent = event;
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
        _lastEvent = event;
      }
    }
  }

  void InputManager::PostUpdate()
  {
    if (_lastEvent)
    {
      EventBus *bus = EventBus::GetInstance();

      if (auto keyEvent = std::dynamic_pointer_cast<KeyEvent>(_lastEvent))
      {
        {
          keyEvent = std::static_pointer_cast<KeyEvent>(_lastEvent);
          if (keyEvent->isHandled && keyEvent->action == KeyEvent::Action::PRESSED)
          {
            std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
            event->key = keyEvent->key;
            event->action = KeyEvent::Action::RELEASED;
            bus->Dispatch(event);
          }
        }
      }
      else if (auto mouseEvent = std::dynamic_pointer_cast<MouseButtonEvent>(_lastEvent))
      {
        mouseEvent = std::static_pointer_cast<MouseButtonEvent>(_lastEvent);
        if (mouseEvent->isHandled && mouseEvent->action == MouseButtonEvent::Action::PRESSED)
        {
          std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
          event->button = mouseEvent->button;
          event->action = MouseButtonEvent::Action::RELEASED;
          bus->Dispatch(event);
        }
      }
      _lastEvent = nullptr;
    }
  }
} // namespace Base
