#pragma once

#include "base/input/MouseState.hpp"
namespace Base
{
  class InputManager
  {
    friend class Game;

  private:
    static InputManager *_instance;
    MouseState _mouseState;
    void Update();

  public:
    ~InputManager();
    [[nodiscard]] static InputManager *GetInstance();
    [[nodiscard]] MouseState *GetMouseState();
  };
} // namespace Base
