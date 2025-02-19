#pragma once
#include "base/Component.hpp"
#include <functional>
#include <unordered_map>

namespace Base
{

  struct BASEAPI InputComponent : public Component
  {
    std::unordered_map<int, std::function<void()>> keyDownBinds;
    std::unordered_map<int, std::function<void()>> keyReleasedBinds;
    std::unordered_map<int, std::function<void()>> keyPressedBinds;
    std::unordered_map<int, std::function<void()>> mousePressedBinds;
    std::unordered_map<int, std::function<void()>> mouseDownBinds;

    void BindKeyDown(int key, std::function<void()> action)
    {
      if (keyDownBinds.find(key) == keyDownBinds.end())
      {
        keyDownBinds[key] = action;
      }
    }

    void BindKeyReleased(int key, std::function<void()> action)
    {
      if (keyReleasedBinds.find(key) == keyReleasedBinds.end())
      {
        keyReleasedBinds[key] = action;
      }
    }

    void BindKeyPressed(int key, std::function<void()> action)
    {
      if (keyPressedBinds.find(key) == keyPressedBinds.end())
      {
        keyPressedBinds[key] = action;
      }
    }
  };
} // namespace Base
