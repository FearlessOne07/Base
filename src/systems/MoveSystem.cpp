#include "base/systems/MoveSystem.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/ABBComponent.hpp"
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

        // Handle collisions for ABB entities
        ABBComponent *abbcmp = nullptr;
        bool hasABBComp = (abbcmp = e->GetComponent<ABBComponent>());
        mc->position.x = mc->position.x + (mc->velocity.x * mc->speed * dt);

        if (hasABBComp)
        {
          HandleCollisions(e, 0, entitymanager);
        }

        mc->position.y = mc->position.y + (mc->velocity.y * mc->speed * dt);
        if (hasABBComp)
        {
          HandleCollisions(e, 1, entitymanager);
        }

        if (hasABBComp && abbcmp)
        {
          abbcmp->oldBoundingBox = abbcmp->boundingBox;
          abbcmp->boundingBox.x = mc->position.x;
          abbcmp->boundingBox.y = mc->position.y;
        }
      }
    }
  }

  void MoveSystem::HandleCollisions(std::shared_ptr<Entity> e, int axis, EntityManager *entityManager)
  {
    std::vector<std::shared_ptr<Entity>> entites = entityManager->Query<ABBComponent>();

    ABBComponent *abbcmp1 = e->GetComponent<ABBComponent>();
    MoveComponent *mvcmp1 = e->GetComponent<MoveComponent>();

    for (auto &e2 : entites)
    {
      if (e != e2)
      {
        ABBComponent *abbcmp2 = e2->GetComponent<ABBComponent>();

        if (CheckCollisionRecs(abbcmp2->boundingBox, abbcmp1->boundingBox))
        {
          if (axis == 0)
          {
            if (abbcmp1->oldBoundingBox.x + abbcmp1->oldBoundingBox.width <= abbcmp2->boundingBox.x)
            {
              mvcmp1->velocity.x = 0;
              mvcmp1->position.x = abbcmp2->boundingBox.x - abbcmp1->boundingBox.width;
            }
            else if (                                                                          //
              abbcmp1->oldBoundingBox.x >= abbcmp2->boundingBox.x + abbcmp2->boundingBox.width //
            )
            {
              mvcmp1->velocity.x = 0;
              mvcmp1->position.x = abbcmp2->boundingBox.x + abbcmp2->boundingBox.width;
            }
          }
          else
          {
            if (abbcmp1->oldBoundingBox.y + abbcmp1->oldBoundingBox.height <= abbcmp2->boundingBox.y)
            {
              mvcmp1->velocity.y = 0;
              mvcmp1->position.y = abbcmp2->boundingBox.y - abbcmp1->boundingBox.height;
            }
            else if (                                                                           //
              abbcmp1->oldBoundingBox.y >= abbcmp2->boundingBox.y + abbcmp2->boundingBox.height //
            )
            {
              mvcmp1->velocity.y = 0;
              mvcmp1->position.y = abbcmp2->boundingBox.y + abbcmp2->boundingBox.height;
            }
          }
        }
      }
      abbcmp1->boundingBox.y = mvcmp1->position.y;
      abbcmp1->boundingBox.x = mvcmp1->position.x;
    }
  }
} // namespace Base
