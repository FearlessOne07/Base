#include "base/input/InputManager.hpp"
#include "base/input/MouseState.hpp"
#include "raylib/raylib.h"

namespace Base
{
  InputManager *InputManager::_instance = nullptr;

  InputManager::~InputManager()
  {
    if (_instance)
    {
      delete _instance;
      _instance = nullptr;
    }
  }

  [[nodiscard]] InputManager *InputManager::GetInstance()
  {
    if (_instance)
    {
      return _instance;
    }
    else
    {
      _instance = new InputManager;
      return _instance;
    }
  }

  MouseState *InputManager::GetMouseState()
  {
    return &_mouseState;
  }

  void InputManager::Update()
  {
    // Update Mouse State
    _mouseState = MouseState();

    // Button Down
    _mouseState.mouseLeftDown.active = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    _mouseState.mouseRightDown.active = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

    // Button Pressed
    _mouseState.mouseLeftPressed.active = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    _mouseState.mouseRightPressed.active = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
  }
} // namespace Base
