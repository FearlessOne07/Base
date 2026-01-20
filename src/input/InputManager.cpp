#include "internal/input/InputManager.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/input/Keys.hpp"
#include "base/scenes/signals/SceneLayerPausedSignal.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/SceneSuspendedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "internal/rendering/Renderer.hpp"
#include <memory>
#include <ranges>

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
    for (uint16_t keyInt = static_cast<uint16_t>(Key::Null); keyInt < static_cast<uint16_t>(Key::Count); keyInt++)
    {
      Key key = static_cast<Key>(keyInt);

      // Key held
      if (Renderer::GetWindowKeyState(key))
      {
        // If this is the first time the key is being registered and it haasn't been handled as a PRESS ...
        if (_keyStates[keyInt].Position == KeyPosition::Up && !_keyStates[keyInt].Handled)
        {
          // Add key to held keys
          _keyStates[keyInt].Position = KeyPosition::Pressed;

          // create and dispatch a PRESSED event
          std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
          event->Key = key;
          event->action = KeyEvent::Action::Pressed;
          _lastEvent = event;
          DispatchEvent(event);

          // If event comes back as handled, add it to handled key PRESSES
          if (event->isHandled)
          {
            _keyStates[keyInt].Handled = true;
          }
        }
        // If the key has been pressed before and hasn't been handled
        else if (_keyStates[keyInt].Position == KeyPosition::Pressed && !_keyStates[keyInt].Handled)
        {
          // It is now HELD
          _keyStates[keyInt].Position = KeyPosition::Down;

          // Dispatch a HELD event
          std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
          event->Key = key;
          event->action = KeyEvent::Action::Held;
          DispatchEvent(event);

          // Set this as the last HELD key
          _lastEvent = event;
        }
      }
      // If the key is Released
      else
      {
        // If The Key was previously active
        if ( //
          (_keyStates[keyInt].Position == KeyPosition::Down || _keyStates[keyInt].Position == KeyPosition::Down) &&
          _keyStates[keyInt].Handled //
        )
        {
          // Reset Key
          _keyStates[keyInt].Position = KeyPosition::Up;
          _keyStates[keyInt].Handled = false;

          // Dispath a RELEASED key event
          std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
          event->Key = key;
          event->action = KeyEvent::Action::Released;
          DispatchEvent(event);

          _lastEvent = event;
        }
      }
    }

    ////// Same Steps for as key events ///////
    {
      for (uint8_t keyInt = static_cast<uint8_t>(MouseKey::Null); keyInt < static_cast<uint8_t>(MouseKey::Count);
           keyInt++)
      {
        MouseKey key = static_cast<MouseKey>(keyInt);

        if (Renderer::GetWindowMouseButtonState(key))
        {
          if (_mouseKeyStates[keyInt].Position == KeyPosition::Up && !_mouseKeyStates[keyInt].Handled)
          {
            _mouseKeyStates[keyInt].Position = KeyPosition::Pressed;

            std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
            event->Button = key;
            event->action = KeyEvent::Action::Pressed;
            _lastEvent = event;
            DispatchEvent(event);

            if (event->isHandled)
            {
              _mouseKeyStates[keyInt].Handled = true;
            }
          }
          else if (_mouseKeyStates[keyInt].Position == KeyPosition::Pressed && !_mouseKeyStates[keyInt].Handled)
          {
            _mouseKeyStates[keyInt].Position = KeyPosition::Down;

            std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
            event->Button = key;
            event->action = KeyEvent::Action::Held;
            DispatchEvent(event);
            if (event->isHandled)
            {
              _mouseKeyStates[keyInt].Handled = true;
            }
            _lastEvent = event;
          }
        }
        else
        {
          if ( //
            (_mouseKeyStates[keyInt].Position == KeyPosition::Down ||
             _mouseKeyStates[keyInt].Position == KeyPosition::Pressed) &&
            _mouseKeyStates[keyInt].Handled //
          )
          {
            _mouseKeyStates[keyInt].Position = KeyPosition::Up;
            _mouseKeyStates[keyInt].Handled = false;
            std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
            event->Button = key;
            event->action = KeyEvent::Action::Released;
            DispatchEvent(event);
            _lastEvent = event;
          }
        }
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
      {
        auto held = std::views::iota(std::size_t{0}, _keyStates.size()) |
                    std::views::filter([&](std::size_t i) { return _keyStates[i].Position == KeyPosition::Down; }) |
                    std::views::transform([&](std::size_t i) -> std::pair<size_t, KeyState &> {
                      return std::pair<std::size_t, KeyState &>{i, _keyStates[i]};
                    });

        for (auto [key, state] : held)
        {
          std::shared_ptr<KeyEvent> event = std::make_shared<KeyEvent>();
          event->Key = static_cast<Key>(key);
          event->action = InputEvent::Action::Released;
          DispatchEvent(event);
          state.Position = KeyPosition::Up;
          state.Handled = false;
        }
      }
      {

        auto held =
          std::views::iota(std::size_t{0}, _mouseKeyStates.size()) |
          std::views::filter([&](std::size_t i) { return _mouseKeyStates[i].Position == KeyPosition::Down; }) |
          std::views::transform([&](std::size_t i) -> std::pair<size_t, KeyState &> {
            return std::pair<std::size_t, KeyState &>{i, _mouseKeyStates[i]};
          });

        for (auto [key, state] : held)
        {
          std::shared_ptr<MouseButtonEvent> event = std::make_shared<MouseButtonEvent>();
          event->Button = static_cast<MouseKey>(key);
          event->action = InputEvent::Action::Released;
          DispatchEvent(event);
          state.Position = KeyPosition::Up;
          state.Handled = false;
        }
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
