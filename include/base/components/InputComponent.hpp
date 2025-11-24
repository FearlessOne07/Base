#pragma once
#include "Component.hpp"
#include "base/input/Keys.hpp"
#include "base/input/MouseButtons.hpp"
#include <functional>
#include <unordered_map>

namespace Base
{

  class InputComponent : public Component
  {
  public:
    std::unordered_map<Key, std::function<void()>> keyDownBinds;
    std::unordered_map<Key, std::function<void()>> keyReleasedBinds;
    std::unordered_map<Key, std::function<void()>> keyPressedBinds;
    std::unordered_map<MouseKey, std::function<void()>> mousePressedBinds;
    std::unordered_map<MouseKey, std::function<void()>> mouseDownBinds;
    std::unordered_map<MouseKey, std::function<void()>> mouseReleasedBinds;

    void BindKeyDown(Key key, std::function<void()> action);
    void BindKeyReleased(Key key, std::function<void()> action);
    void BindKeyPressed(Key key, std::function<void()> action);

    void BindMouseButtonPressed(MouseKey button, std::function<void()> action);
    void BindMouseButtonReleased(MouseKey button, std::function<void()> action);
    void BindMouseButtonDown(MouseKey button, std::function<void()> action);
  };
} // namespace Base
