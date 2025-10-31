#include "internal/input/InputManager.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/scenes/signals/SceneLayerPausedSignal.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/SceneSuspendedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "raylib.h"
#include <algorithm>
#include <memory>

namespace Base
{
  void InputManager::Init()
  {
    // Sunscribe to Scene suspension / Popping signal
    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<SceneSuspendedSignal>([this](const std::shared_ptr<Signal> &signal) {
      this->ResetInput(signal); //
    });

    bus->SubscribeSignal<ScenePoppedSignal>([this](const std::shared_ptr<Signal> &signal) {
      this->ResetInput(signal); //
    });

    bus->SubscribeSignal<SceneLayerPausedSignal>([this](const std::shared_ptr<Signal> &signal) {
      this->ResetInput(signal); //
    });
  }

  void InputManager::PollAndDispatch()
  {
    // Track key state
    for (int key = KEY_NULL; key <= KEY_KB_MENU; ++key)
    {
      // Key held
      if (IsKeyDown(key))
      {
        // If this is the first time the key is being registered and it haasn't been handled as a PRESS ...
        if (!_heldKeys.contains(key) && std::ranges::find(_handledKeyPresses, key) == _handledKeyPresses.end())
        {
          // Add key to held keys
          _heldKeys[key] = 1;

          // create and dispatch a PRESSED event
          std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
          event->key = key;
          event->action = KeyEvent::Action::Pressed;
          _lastEvent = event;
          DispatchEvent(event);

          // If event comes back as handled, add it to handled key PRESSES
          if (event->isHandled)
          {
            _handledKeyPresses.push_back(key);
          }
        }
        // If the key has been pressed before...
        else
        {
          // And hasn;t been handled
          if (std::ranges::find(_handledKeyPresses, key) == _handledKeyPresses.end())
          {
            // It is now HELD
            ++_heldKeys.at(key);

            // Dispatch a HELD event
            std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
            event->key = key;
            event->action = KeyEvent::Action::Held;
            DispatchEvent(event);

            // Set this as the last HELD key
            _lastEvent = event;
          }
        }
      }
      // If the key is Released
      else if (IsKeyReleased(key))
      {
        // Erase it from held keys
        _heldKeys.erase(key);

        // if it has been handled as a PRESS
        auto it = std::ranges::find(_handledKeyPresses, key);

        if (it != _handledKeyPresses.end())
        {
          // Erase it from handled key PRESSES
          _handledKeyPresses.erase(it);
        }

        // Dispath a RELEASED key event
        std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
        event->key = key;
        event->action = KeyEvent::Action::Released;
        DispatchEvent(event);

        _lastEvent = event;
      }
    }

    ////// Same Steps for as key events ///////
    for (int btn = 0; btn <= MOUSE_BUTTON_RIGHT; ++btn)
    {
      if (IsMouseButtonDown(btn))
      {
        if (!_heldMouseBtns.contains(btn) && std::ranges::find(_handledMousePresses, btn) == _handledMousePresses.end())
        {
          _heldMouseBtns[btn] = 1;
          std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
          event->button = btn;
          event->action = MouseButtonEvent::Action::Pressed;
          DispatchEvent(event);
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
            event->action = MouseButtonEvent::Action::Held;
            DispatchEvent(event);
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
        event->action = MouseButtonEvent::Action::Released;
        DispatchEvent(event);
        _lastEvent = event;
      }
    }
  }

  void InputManager::PostUpdate()
  {
    if (_lastEvent)
    {

      // If the last event was a PRESSED event, dispatch a synthetic RELEASED event
      // so that listeners can reverse the effects of the press
      if (auto keyEvent = std::dynamic_pointer_cast<KeyEvent>(_lastEvent))
      {
        keyEvent = std::static_pointer_cast<KeyEvent>(_lastEvent);
        if (keyEvent->isHandled && keyEvent->action == KeyEvent::Action::Pressed)
        {
          std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
          event->key = keyEvent->key;
          event->action = KeyEvent::Action::Released;
          DispatchEvent(event);
        }
      }
      else if (auto mouseEvent = std::dynamic_pointer_cast<MouseButtonEvent>(_lastEvent))
      {
        mouseEvent = std::static_pointer_cast<MouseButtonEvent>(_lastEvent);
        if (mouseEvent->isHandled && mouseEvent->action == MouseButtonEvent::Action::Pressed)
        {
          std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
          event->button = mouseEvent->button;
          event->action = MouseButtonEvent::Action::Released;
          DispatchEvent(event);
        }
      }
      _lastEvent = nullptr;
    }
  }

  void InputManager::RegisterListener(InputListener *listener)
  {
    _listenrs.push_back(listener);
  }

  // Called when A scene is popped or Suspended
  void InputManager::ResetInput(const std::shared_ptr<Signal> &sig)
  {
    // For any Held Keys / Mouse buttons, when a scene is changed, dispatch synthetic RELEASED events
    // so effects of the HELD events can be reversed
    if ( //
      std::dynamic_pointer_cast<SceneSuspendedSignal>(sig) || std::dynamic_pointer_cast<ScenePoppedSignal>(sig) ||
      std::dynamic_pointer_cast<SceneLayerPausedSignal>(sig) //
    )
    {
      for (auto it = _heldKeys.begin(); it != _heldKeys.end();)
      {
        std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
        event->key = it->first;
        event->action = InputEvent::Action::Released;
        DispatchEvent(event);
        it = _heldKeys.erase(it);
      }

      for (auto it = _heldMouseBtns.begin(); it != _heldMouseBtns.end();)
      {
        std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
        event->button = it->first;
        event->action = InputEvent::Action::Released;
        DispatchEvent(event);
        it = _heldMouseBtns.erase(it);
      }
    }
  }

  void InputManager::DispatchEvent(std::shared_ptr<InputEvent> event)
  {
    // Dispatch events to listeners
    for (auto &listener : _listenrs)
    {
      listener->OnInputEvent(event);
      if (event->isHandled)
      {
        break;
      }
    }
  }
} // namespace Base
