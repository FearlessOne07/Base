#include "base/systems/InputSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/components/InputComponent.hpp"
#include "raylib.h"

namespace Base
{
  void InputSystem::Update(float dt, EntityManager *entitymanager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entitymanager->Query<InputComponent>();

    for (auto &e : entities)
    {
      InputComponent *inpcmp = e->GetComponent<InputComponent>();

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

      for (auto &[key, action] : inpcmp->mouseDownBinds)
      {
        if (IsMouseButtonDown(key))
        {
          action();
        }
      }

      for (auto &[key, action] : inpcmp->mousePressedBinds)
      {
        if (IsMouseButtonPressed(key))
        {
          action();
        }
      }

      for (auto &[key, action] : inpcmp->keyReleasedBinds)
      {
        if (IsKeyReleased(key))
        {
          action();
        }
      }
    }
  }
} // namespace Base
