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

    void BindKeyDown(int key, std::function<void()> action);
    void BindKeyReleased(int key, std::function<void()> action);
    void BindKeyPressed(int key, std::function<void()> action);

    void BindMouseButtonPressed(int key, std::function<void()> action);
    void BindMouseButtonDown(int key, std::function<void()> action);
  };
} // namespace Base
