#include "base/systems/InputSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/components/InputComponent.hpp"
#include "base/input/InputManager.hpp"
#include "base/input/MouseState.hpp"
#include "raylib.h"

namespace Base
{
  void InputSystem::Update(float dt, EntityManager *entitymanager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entitymanager->Query<InputComponent>();

    for (std::shared_ptr<Entity> &e : entities)
    {

      auto *inpcmp = e->GetComponent<InputComponent>();

      for (auto &[key, action] : inpcmp->keyPressedBinds)
      {
        if (IsKeyPressed(key))
        {
          action();
        }
      }

      for (auto &[key, action] : inpcmp->keyDownBinds)
      {
        if (IsKeyDown(key))
        {
          action();
        }
      }

      InputManager *inpMan = InputManager::GetInstance();
      MouseState *mouseState = inpMan->GetMouseState();

      for (auto &[key, action] : inpcmp->mouseDownBinds)
      {
        if (key == MOUSE_BUTTON_LEFT)
        {
          if (mouseState->mouseLeftDown.active && !mouseState->mouseLeftDown.handled)
          {
            action();
            mouseState->mouseLeftDown.handled = true;
          }
        }
        else if (key == MOUSE_BUTTON_RIGHT)
        {
          if (mouseState->mouseRightDown.active && !mouseState->mouseRightDown.handled)
          {
            action();
            mouseState->mouseRightDown.handled = true;
          }
        }
      }

      for (auto &[key, action] : inpcmp->mousePressedBinds)
      {
        if (key == MOUSE_BUTTON_LEFT)
        {
          if (mouseState->mouseLeftPressed.active && !mouseState->mouseLeftPressed.handled)
          {
            action();
            mouseState->mouseLeftPressed.handled = true;
          }
        }
        else if (key == MOUSE_BUTTON_RIGHT)
        {
          if (mouseState->mouseRightPressed.active && !mouseState->mouseRightPressed.handled)
          {
            action();
            mouseState->mouseRightPressed.handled = true;
          }
        }
      }

      for (auto &[key, action] : inpcmp->keyReleasedBinds)
      {
        if (IsKeyReleased(key))
        {
          action();
        }
      }

      for (auto &[key, action] : inpcmp->mouseReleasedBinds)
      {
        if (IsMouseButtonUp(key))
        {
          action();
        }
      }
    }
  }
} // namespace Base
