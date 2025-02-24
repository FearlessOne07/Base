#include "base/components/InputComponent.hpp"

namespace Base
{

  void InputComponent::BindKeyDown(int key, std::function<void()> action)
  {
    if (keyDownBinds.find(key) == keyDownBinds.end())
    {
      keyDownBinds[key] = action;
    }
  }

  void InputComponent::BindKeyReleased(int key, std::function<void()> action)
  {
    if (keyReleasedBinds.find(key) == keyReleasedBinds.end())
    {
      keyReleasedBinds[key] = action;
    }
  }

  void InputComponent::BindKeyPressed(int key, std::function<void()> action)
  {
    if (keyPressedBinds.find(key) == keyPressedBinds.end())
    {
      keyPressedBinds[key] = action;
    }
  }
} // namespace Base
