#include <utility>
#include "base/components/InputComponent.hpp"

namespace Base
{

  void InputComponent::BindKeyDown(int key, std::function<void()> action)
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

  void InputComponent::BindMouseButtonDown(int key, std::function<void()> action)
  {
    if (mouseDownBinds.find(key) == mouseDownBinds.end())
    {
      mouseDownBinds[key] = std::move(action);
    }
  }
} // namespace Base
