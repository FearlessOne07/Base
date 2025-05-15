#include "base/components/InputComponent.hpp"
#include "base/ui/elements/UIButton.hpp"
#include <utility>

namespace Base
{
  void InputComponent::BindKeyDown(int key, AntagonisticFunction action)
  {
    if (keyDownBinds.find(key) == keyDownBinds.end())
    {
      keyDownBinds[key] = std::move(action);
    }
  }

  void InputComponent::BindKeyReleased(int key, std::function<void()> action)
  {
    if (keyReleasedBinds.find(key) == keyReleasedBinds.end())
    {
      keyReleasedBinds[key] = std::move(action);
    }
  }

  void InputComponent::BindKeyPressed(int key, std::function<void()> action)
  {
    if (keyPressedBinds.find(key) == keyPressedBinds.end())
    {
      keyPressedBinds[key] = std::move(action);
    }
  }
  void InputComponent::BindMouseButtonPressed(int key, std::function<void()> action)
  {
    if (mousePressedBinds.find(key) == mousePressedBinds.end())
    {
      mousePressedBinds[key] = std::move(action);
    }
  }

  void InputComponent::BindMouseButtonDown(int key, AntagonisticFunction action)
  {
    if (mouseDownBinds.find(key) == mouseDownBinds.end())
    {
      mouseDownBinds[key] = std::move(action);
    }
  }

  void InputComponent::BindMouseButtonReleased(int key, std::function<void()> action)
  {
    if (mouseReleasedBinds.find(key) == mouseReleasedBinds.end())
    {
      mouseReleasedBinds[key] = std::move(action);
    }
  }
} // namespace Base
