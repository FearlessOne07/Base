#include "base/systems/MoveSystem.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/ABBComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib.h"
#include "raymath.h"
#include <memory>
#include <vector>

namespace Base
{
  void MoveSystem::Update(float dt, EntityManager *entitymanager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entitymanager->Query<MoveComponent, TransformComponent>();

    for (std::shared_ptr<Entity> &e : entities)
    {
      if (e)
      {
        auto *transcmp = e->GetComponent<TransformComponent>();
        auto *mc = e->GetComponent<MoveComponent>();

        if (Vector2Length(mc->targetVelocity) > 0)
        {
          mc->targetVelocity = Vector2Normalize(mc->targetVelocity);
        }
        mc->velocity = Vector2Lerp(mc->velocity, mc->targetVelocity, mc->acceleration * dt);

        if (abs(mc->velocity.x) < 5e-5)
        {
          mc->velocity.x = 0;
        }

        if (abs(mc->velocity.y) < 5e-5)
        {
          mc->velocity.y = 0;
        }

        bool hasABBComp = e->HasComponent<ABBComponent>();
        transcmp->position.x = transcmp->position.x + (mc->velocity.x * mc->speed * dt);

        if (hasABBComp)
        {
          HandleCollisions(e, 0, entitymanager);
        }

        transcmp->position.y = transcmp->position.y + (mc->velocity.y * mc->speed * dt);
        if (hasABBComp)
        {
          HandleCollisions(e, 1, entitymanager);
        }

        if (hasABBComp)
        {
          auto *abbcmp = e->GetComponent<ABBComponent>();
          abbcmp->oldBoundingBox = abbcmp->boundingBox;
          abbcmp->boundingBox.x = transcmp->position.x;
          abbcmp->boundingBox.y = transcmp->position.y;
        }
      }
    }
  }

  void MoveSystem::HandleCollisions(std::shared_ptr<Entity> &e, int axis, EntityManager *entityManager)
  {
    std::vector<std::shared_ptr<Entity>> entites = entityManager->Query<ABBComponent>();

    auto *abbcmp1 = e->GetComponent<ABBComponent>();
    auto *mvcmp1 = e->GetComponent<MoveComponent>();
    auto *transcmp = e->GetComponent<TransformComponent>();

    for (std::shared_ptr<Entity> &e2 : entites)
    {
      if (e != e2)
      {
        auto *abbcmp2 = e2->GetComponent<ABBComponent>();

        if (CheckCollisionRecs(abbcmp2->boundingBox, abbcmp1->boundingBox))
        {
          if (axis == 0)
          {
            if (abbcmp1->oldBoundingBox.x + abbcmp1->oldBoundingBox.width <= abbcmp2->boundingBox.x)
            {
              mvcmp1->velocity.x = 0;
              transcmp->position.x = abbcmp2->boundingBox.x - abbcmp1->boundingBox.width;
            }
            else if (                                                                          //
              abbcmp1->oldBoundingBox.x >= abbcmp2->boundingBox.x + abbcmp2->boundingBox.width //
            )
            {
              mvcmp1->velocity.x = 0;
              transcmp->position.x = abbcmp2->boundingBox.x + abbcmp2->boundingBox.width;
            }
          }
          else
          {
            if (abbcmp1->oldBoundingBox.y + abbcmp1->oldBoundingBox.height <= abbcmp2->boundingBox.y)
            {
              mvcmp1->velocity.y = 0;
              transcmp->position.y = abbcmp2->boundingBox.y - abbcmp1->boundingBox.height;
            }
            else if (                                                                           //
              abbcmp1->oldBoundingBox.y >= abbcmp2->boundingBox.y + abbcmp2->boundingBox.height //
            )
            {
              mvcmp1->velocity.y = 0;
              transcmp->position.y = abbcmp2->boundingBox.y + abbcmp2->boundingBox.height;
            }
          }
        }
      }
      abbcmp1->boundingBox.y = transcmp->position.y;
      abbcmp1->boundingBox.x = transcmp->position.x;
    }
  }
} // namespace Base
