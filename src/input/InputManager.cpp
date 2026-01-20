#include "internal/input/InputManager.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/input/Keys.hpp"
#include "base/input/MouseButtons.hpp"
#include "base/scenes/signals/SceneLayerPausedSignal.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/SceneSuspendedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "internal/rendering/Renderer.hpp"
#include <algorithm>
#include <memory>

namespace Base
{
  static Key FromGLFWKey(int glfwKey)
  {
    switch (glfwKey)
    {
    case GLFW_KEY_UNKNOWN:
      return Key::Null;

    case GLFW_KEY_APOSTROPHE:
      return Key::Apostrophe;
    case GLFW_KEY_COMMA:
      return Key::Comma;
    case GLFW_KEY_MINUS:
      return Key::Minus;
    case GLFW_KEY_PERIOD:
      return Key::Period;
    case GLFW_KEY_SLASH:
      return Key::Slash;

    case GLFW_KEY_0:
      return Key::Zero;
    case GLFW_KEY_1:
      return Key::One;
    case GLFW_KEY_2:
      return Key::Two;
    case GLFW_KEY_3:
      return Key::Three;
    case GLFW_KEY_4:
      return Key::Four;
    case GLFW_KEY_5:
      return Key::Five;
    case GLFW_KEY_6:
      return Key::Six;
    case GLFW_KEY_7:
      return Key::Seven;
    case GLFW_KEY_8:
      return Key::Eight;
    case GLFW_KEY_9:
      return Key::Nine;

    case GLFW_KEY_SEMICOLON:
      return Key::Semicolon;
    case GLFW_KEY_EQUAL:
      return Key::Equal;

    case GLFW_KEY_A:
      return Key::A;
    case GLFW_KEY_B:
      return Key::B;
    case GLFW_KEY_C:
      return Key::C;
    case GLFW_KEY_D:
      return Key::D;
    case GLFW_KEY_E:
      return Key::E;
    case GLFW_KEY_F:
      return Key::F;
    case GLFW_KEY_G:
      return Key::G;
    case GLFW_KEY_H:
      return Key::H;
    case GLFW_KEY_I:
      return Key::I;
    case GLFW_KEY_J:
      return Key::J;
    case GLFW_KEY_K:
      return Key::K;
    case GLFW_KEY_L:
      return Key::L;
    case GLFW_KEY_M:
      return Key::M;
    case GLFW_KEY_N:
      return Key::N;
    case GLFW_KEY_O:
      return Key::O;
    case GLFW_KEY_P:
      return Key::P;
    case GLFW_KEY_Q:
      return Key::Q;
    case GLFW_KEY_R:
      return Key::R;
    case GLFW_KEY_S:
      return Key::S;
    case GLFW_KEY_T:
      return Key::T;
    case GLFW_KEY_U:
      return Key::U;
    case GLFW_KEY_V:
      return Key::V;
    case GLFW_KEY_W:
      return Key::W;
    case GLFW_KEY_X:
      return Key::X;
    case GLFW_KEY_Y:
      return Key::Y;
    case GLFW_KEY_Z:
      return Key::Z;

    case GLFW_KEY_LEFT_BRACKET:
      return Key::LeftBracket;
    case GLFW_KEY_BACKSLASH:
      return Key::Backslash;
    case GLFW_KEY_RIGHT_BRACKET:
      return Key::RightBracket;
    case GLFW_KEY_GRAVE_ACCENT:
      return Key::Grave;

    case GLFW_KEY_SPACE:
      return Key::Space;
    case GLFW_KEY_ESCAPE:
      return Key::Escape;
    case GLFW_KEY_ENTER:
      return Key::Enter;
    case GLFW_KEY_TAB:
      return Key::Tab;
    case GLFW_KEY_BACKSPACE:
      return Key::Backspace;
    case GLFW_KEY_INSERT:
      return Key::Insert;
    case GLFW_KEY_DELETE:
      return Key::Delete;

    case GLFW_KEY_RIGHT:
      return Key::Right;
    case GLFW_KEY_LEFT:
      return Key::Left;
    case GLFW_KEY_DOWN:
      return Key::Down;
    case GLFW_KEY_UP:
      return Key::Up;

    case GLFW_KEY_PAGE_UP:
      return Key::PageUp;
    case GLFW_KEY_PAGE_DOWN:
      return Key::PageDown;
    case GLFW_KEY_HOME:
      return Key::Home;
    case GLFW_KEY_END:
      return Key::End;

    case GLFW_KEY_CAPS_LOCK:
      return Key::CapsLock;
    case GLFW_KEY_SCROLL_LOCK:
      return Key::ScrollLock;
    case GLFW_KEY_NUM_LOCK:
      return Key::NumLock;
    case GLFW_KEY_PRINT_SCREEN:
      return Key::PrintScreen;
    case GLFW_KEY_PAUSE:
      return Key::Pause;

    case GLFW_KEY_F1:
      return Key::F1;
    case GLFW_KEY_F2:
      return Key::F2;
    case GLFW_KEY_F3:
      return Key::F3;
    case GLFW_KEY_F4:
      return Key::F4;
    case GLFW_KEY_F5:
      return Key::F5;
    case GLFW_KEY_F6:
      return Key::F6;
    case GLFW_KEY_F7:
      return Key::F7;
    case GLFW_KEY_F8:
      return Key::F8;
    case GLFW_KEY_F9:
      return Key::F9;
    case GLFW_KEY_F10:
      return Key::F10;
    case GLFW_KEY_F11:
      return Key::F11;
    case GLFW_KEY_F12:
      return Key::F12;

    case GLFW_KEY_LEFT_SHIFT:
      return Key::LeftShift;
    case GLFW_KEY_LEFT_CONTROL:
      return Key::LeftControl;
    case GLFW_KEY_LEFT_ALT:
      return Key::LeftAlt;
    case GLFW_KEY_LEFT_SUPER:
      return Key::LeftSuper;

    case GLFW_KEY_RIGHT_SHIFT:
      return Key::RightShift;
    case GLFW_KEY_RIGHT_CONTROL:
      return Key::RightControl;
    case GLFW_KEY_RIGHT_ALT:
      return Key::RightAlt;
    case GLFW_KEY_RIGHT_SUPER:
      return Key::RightSuper;

    case GLFW_KEY_MENU:
      return Key::KbMenu;

    case GLFW_KEY_KP_0:
      return Key::Keypad0;
    case GLFW_KEY_KP_1:
      return Key::Keypad1;
    case GLFW_KEY_KP_2:
      return Key::Keypad2;
    case GLFW_KEY_KP_3:
      return Key::Keypad3;
    case GLFW_KEY_KP_4:
      return Key::Keypad4;
    case GLFW_KEY_KP_5:
      return Key::Keypad5;
    case GLFW_KEY_KP_6:
      return Key::Keypad6;
    case GLFW_KEY_KP_7:
      return Key::Keypad7;
    case GLFW_KEY_KP_8:
      return Key::Keypad8;
    case GLFW_KEY_KP_9:
      return Key::Keypad9;

    case GLFW_KEY_KP_DECIMAL:
      return Key::KeypadDecimal;
    case GLFW_KEY_KP_DIVIDE:
      return Key::KeypadDivide;
    case GLFW_KEY_KP_MULTIPLY:
      return Key::KeypadMultiply;
    case GLFW_KEY_KP_SUBTRACT:
      return Key::KeypadSubtract;
    case GLFW_KEY_KP_ADD:
      return Key::KeypadAdd;
    case GLFW_KEY_KP_ENTER:
      return Key::KeypadEnter;
    case GLFW_KEY_KP_EQUAL:
      return Key::KeypadEqual;

    default:
      return Key::Null;
    }
  }

  static MouseKey FromGLFWMouseButton(int glfwButton)
  {
    switch (glfwButton)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
      return MouseKey::Left;
    case GLFW_MOUSE_BUTTON_RIGHT:
      return MouseKey::Right;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      return MouseKey::Middle;
    case GLFW_MOUSE_BUTTON_4:
      return MouseKey::Side;
    case GLFW_MOUSE_BUTTON_5:
      return MouseKey::Extra;
    case GLFW_MOUSE_BUTTON_6:
      return MouseKey::Forward;
    case GLFW_MOUSE_BUTTON_7:
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

    // Init Renderer Call Backs
    Renderer::SetKeyCallback(
      [this](int key, int scancode, int action, int mods) { DispatchKeyEvent(key, scancode, action, mods); } //
    );
    Renderer::SetMouseButtonCallback(
      [this](int button, int action, int mods) { DispatchMouseEvent(button, action, mods); } //
    );
  }

  void InputManager::DispatchKeyEvent(int keyGLFW, int scancode, int action, int mods)
  {
    // Track key state
    {
      // Key held
      if (action == GLFW_PRESS)
      {
        Key key = FromGLFWKey(keyGLFW);

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
      else if (action == GLFW_RELEASE)
      {

        Key key = FromGLFWKey(keyGLFW);

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
  }

  void InputManager::DispatchMouseEvent(int btnGLFW, int action, int mods)
  {
    ////// Same Steps for as key events ///////
    {
      if (action == GLFW_PRESS)
      {
        MouseKey btn = FromGLFWMouseButton(btnGLFW);
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
      else if (action == GLFW_RELEASE)
      {
        MouseKey btn = FromGLFWMouseButton(btnGLFW);
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
