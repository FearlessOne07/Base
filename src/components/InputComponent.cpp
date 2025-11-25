#include "base/components/InputComponent.hpp"
#include <utility>

namespace Base
{
  void InputComponent::BindKeyDown(Key key, std::function<void()> action)
  {
    if (keyDownBinds.find(key) == keyDownBinds.end())
    {
      keyDownBinds[key] = std::move(action);
    }
  }

  void InputComponent::BindKeyReleased(Key key, std::function<void()> action)
  {
    if (keyReleasedBinds.find(key) == keyReleasedBinds.end())
    {
      keyReleasedBinds[key] = std::move(action);
    }
  }

  void InputComponent::BindKeyPressed(Key key, std::function<void()> action)
  {
    if (keyPressedBinds.find(key) == keyPressedBinds.end())
    {
      keyPressedBinds[key] = std::move(action);
    }
  }
  void InputComponent::BindMouseButtonPressed(MouseKey button, std::function<void()> action)
  {
    if (mousePressedBinds.find(button) == mousePressedBinds.end())
    {
      mousePressedBinds[button] = std::move(action);
    }
  }

  void InputComponent::BindMouseButtonDown(MouseKey button, std::function<void()> action)
  {
    if (mouseDownBinds.find(button) == mouseDownBinds.end())
    {
      mouseDownBinds[button] = std::move(action);
    }
  }

  void InputComponent::BindMouseButtonReleased(MouseKey button, std::function<void()> action)
  {
    if (mouseReleasedBinds.find(button) == mouseReleasedBinds.end())
    {
      mouseReleasedBinds[button] = std::move(action);
    }
  }
} // namespace Base
