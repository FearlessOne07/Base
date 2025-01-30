#include "base/systems/MoveSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/components/MoveComponent.hpp"
#include "raylib.h"
#include "raymath.h"
#include <memory>
#include <vector>

namespace Base
{
  void MoveSystem::Update(float dt, EntityManager *entitymanager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entitymanager->Query<MoveComponent>();
    for (std::shared_ptr<Entity> &e : entities)
    {
      if (e)
      {
        MoveComponent *mc = e->GetComponent<MoveComponent>();

        if (Vector2Length(mc->targetVelocity) > 0)
        {
          Vector2Normalize(mc->targetVelocity);
        }
        mc->velocity = Vector2Lerp(mc->velocity, mc->targetVelocity, mc->acceleration * dt);
        mc->position = Vector2Add(mc->position, Vector2Scale(mc->velocity, mc->speed * dt));
      }
    }
  }
} // namespace Base
