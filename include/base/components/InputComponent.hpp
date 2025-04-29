#pragma once
#include "Component.hpp"
#include <functional>
#include <unordered_map>

namespace Base
{

  struct InputComponent : public Component
  {
    std::unordered_map<int, std::function<void()>> keyDownBinds;
    std::unordered_map<int, std::function<void()>> keyReleasedBinds;
    std::unordered_map<int, std::function<void()>> keyPressedBinds;
    std::unordered_map<int, std::function<void()>> mousePressedBinds;
    std::unordered_map<int, std::function<void()>> mouseDownBinds;
    std::unordered_map<int, std::function<void()>> mouseReleasedBinds;

    void BindKeyDown(int key, std::function<void()> action);
    void BindKeyReleased(int key, std::function<void()> action);
    void BindKeyPressed(int key, std::function<void()> action);

    void BindMouseButtonPressed(int key, std::function<void()> action);
    void BindMouseButtonReleased(int key, std::function<void()> action);
    void BindMouseButtonDown(int key, std::function<void()> action);
  };
} // namespace Base
