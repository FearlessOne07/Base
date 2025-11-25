#include "internal/input/InputManager.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/input/Keys.hpp"
#include "base/input/MouseButtons.hpp"
#include "base/scenes/signals/SceneLayerPausedSignal.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/SceneSuspendedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "raylib.h"
#include <algorithm>
#include <memory>

namespace Base
{
  static Key FromRaylibKey(int raylibKey)
  {
    switch (raylibKey)
    {
    case 0:
      return Key::Null;
    case 39:
      return Key::Apostrophe;
    case 44:
      return Key::Comma;
    case 45:
      return Key::Minus;
    case 46:
      return Key::Period;
    case 47:
      return Key::Slash;
    case 48:
      return Key::Zero;
    case 49:
      return Key::One;
    case 50:
      return Key::Two;
    case 51:
      return Key::Three;
    case 52:
      return Key::Four;
    case 53:
      return Key::Five;
    case 54:
      return Key::Six;
    case 55:
      return Key::Seven;
    case 56:
      return Key::Eight;
    case 57:
      return Key::Nine;
    case 59:
      return Key::Semicolon;
    case 61:
      return Key::Equal;
    case 65:
      return Key::A;
    case 66:
      return Key::B;
    case 67:
      return Key::C;
    case 68:
      return Key::D;
    case 69:
      return Key::E;
    case 70:
      return Key::F;
    case 71:
      return Key::G;
    case 72:
      return Key::H;
    case 73:
      return Key::I;
    case 74:
      return Key::J;
    case 75:
      return Key::K;
    case 76:
      return Key::L;
    case 77:
      return Key::M;
    case 78:
      return Key::N;
    case 79:
      return Key::O;
    case 80:
      return Key::P;
    case 81:
      return Key::Q;
    case 82:
      return Key::R;
    case 83:
      return Key::S;
    case 84:
      return Key::T;
    case 85:
      return Key::U;
    case 86:
      return Key::V;
    case 87:
      return Key::W;
    case 88:
      return Key::X;
    case 89:
      return Key::Y;
    case 90:
      return Key::Z;
    case 91:
      return Key::LeftBracket;
    case 92:
      return Key::Backslash;
    case 93:
      return Key::RightBracket;
    case 96:
      return Key::Grave;
    case 32:
      return Key::Space;
    case 256:
      return Key::Escape;
    case 257:
      return Key::Enter;
    case 258:
      return Key::Tab;
    case 259:
      return Key::Backspace;
    case 260:
      return Key::Insert;
    case 261:
      return Key::Delete;
    case 262:
      return Key::Right;
    case 263:
      return Key::Left;
    case 264:
      return Key::Down;
    case 265:
      return Key::Up;
    case 266:
      return Key::PageUp;
    case 267:
      return Key::PageDown;
    case 268:
      return Key::Home;
    case 269:
      return Key::End;
    case 280:
      return Key::CapsLock;
    case 281:
      return Key::ScrollLock;
    case 282:
      return Key::NumLock;
    case 283:
      return Key::PrintScreen;
    case 284:
      return Key::Pause;
    case 290:
      return Key::F1;
    case 291:
      return Key::F2;
    case 292:
      return Key::F3;
    case 293:
      return Key::F4;
    case 294:
      return Key::F5;
    case 295:
      return Key::F6;
    case 296:
      return Key::F7;
    case 297:
      return Key::F8;
    case 298:
      return Key::F9;
    case 299:
      return Key::F10;
    case 300:
      return Key::F11;
    case 301:
      return Key::F12;
    case 340:
      return Key::LeftShift;
    case 341:
      return Key::LeftControl;
    case 342:
      return Key::LeftAlt;
    case 343:
      return Key::LeftSuper;
    case 344:
      return Key::RightShift;
    case 345:
      return Key::RightControl;
    case 346:
      return Key::RightAlt;
    case 347:
      return Key::RightSuper;
    case 348:
      return Key::KbMenu;
    case 320:
      return Key::Keypad0;
    case 321:
      return Key::Keypad1;
    case 322:
      return Key::Keypad2;
    case 323:
      return Key::Keypad3;
    case 324:
      return Key::Keypad4;
    case 325:
      return Key::Keypad5;
    case 326:
      return Key::Keypad6;
    case 327:
      return Key::Keypad7;
    case 328:
      return Key::Keypad8;
    case 329:
      return Key::Keypad9;
    case 330:
      return Key::KeypadDecimal;
    case 331:
      return Key::KeypadDivide;
    case 332:
      return Key::KeypadMultiply;
    case 333:
      return Key::KeypadSubtract;
    case 334:
      return Key::KeypadAdd;
    case 335:
      return Key::KeypadEnter;
    case 336:
      return Key::KeypadEqual;
    default:
      return Key::Null;
    }
  }

  static MouseKey FromRaylibButton(int raylibButton)
  {
    switch (raylibButton)
    {
    case 0:
      return MouseKey::Left;
    case 1:
      return MouseKey::Right;
    case 2:
      return MouseKey::Middle;
    case 3:
      return MouseKey::Side;
    case 4:
      return MouseKey::Extra;
    case 5:
      return MouseKey::Forward;
    case 6:
      return MouseKey::Back;
    default:
      return MouseKey::Left;
    }
  }

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
    for (int keyRay = KEY_NULL; keyRay <= KEY_KB_MENU; ++keyRay)
    {
      // Key held
      if (IsKeyDown(keyRay))
      {
        Key key = FromRaylibKey(keyRay);

        // If this is the first time the key is being registered and it haasn't been handled as a PRESS ...
        if (!_heldKeys.contains(key) && std::ranges::find(_handledKeyPresses, key) == _handledKeyPresses.end())
        {
          // Add key to held keys
          _heldKeys[key] = 1;

          // create and dispatch a PRESSED event
          std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
          event->Key = key;
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
            event->Key = key;
            event->action = KeyEvent::Action::Held;
            DispatchEvent(event);

            // Set this as the last HELD key
            _lastEvent = event;
          }
        }
      }
      // If the key is Released
      else if (IsKeyReleased(keyRay))
      {

        Key key = FromRaylibKey(keyRay);

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
        event->Key = key;
        event->action = KeyEvent::Action::Released;
        DispatchEvent(event);

        _lastEvent = event;
      }
    }

    ////// Same Steps for as key events ///////
    for (int btnRay = 0; btnRay <= MOUSE_BUTTON_RIGHT; ++btnRay)
    {
      if (IsMouseButtonDown(btnRay))
      {
        MouseKey btn = FromRaylibButton(btnRay);
        if (!_heldMouseBtns.contains(btn) && std::ranges::find(_handledMousePresses, btn) == _handledMousePresses.end())
        {
          _heldMouseBtns[btn] = 1;
          std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
          event->Button = btn;
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
            event->Button = btn;
            event->action = MouseButtonEvent::Action::Held;
            DispatchEvent(event);
            _lastEvent = event;
          }
        }
      }
      else if (IsMouseButtonReleased(btnRay))
      {
        MouseKey btn = FromRaylibButton(btnRay);
        _heldMouseBtns.erase(btn);

        auto it = std::ranges::find(_handledMousePresses, btn);

        if (it != _handledMousePresses.end())
        {
          _handledMousePresses.erase(it);
        }

        std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
        event->Button = btn;
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
          event->Key = keyEvent->Key;
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
          event->Button = mouseEvent->Button;
          event->action = MouseButtonEvent::Action::Released;
          DispatchEvent(event);
        }
      }
      _lastEvent = nullptr;
    }
  }

  void InputManager::RegisterListener(InputListener &listener)
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
        event->Key = it->first;
        event->action = InputEvent::Action::Released;
        DispatchEvent(event);
        it = _heldKeys.erase(it);
      }

      for (auto it = _heldMouseBtns.begin(); it != _heldMouseBtns.end();)
      {
        std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
        event->Button = it->first;
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
