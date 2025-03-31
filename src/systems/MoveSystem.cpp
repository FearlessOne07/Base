#include "base/systems/MoveSystem.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/BoundingBoxComponent.hpp"
#include "base/components/GravityComponent.hpp"
#include "base/components/ImpulseComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
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
        auto *transcmp = e->GetComponent<TransformComponent>();
        auto *mc = e->GetComponent<MoveComponent>();

        if (Vector2Length(mc->targetVelocity) > 0)
        {
          mc->targetVelocity = Vector2Normalize(mc->targetVelocity);
        }

        // Update X velocity
        mc->velocity.x = Lerp(mc->velocity.x, mc->targetVelocity.x, mc->acceleration * dt);

        // Apply an ipulse if present
        if (e->HasComponent<ImpulseComponent>())
        {
          auto *impcmp = e->GetComponent<ImpulseComponent>();
          mc->velocity.x += Vector2Normalize(impcmp->direction).x;
        }

        // Update y velocity
        if (e->HasComponent<Base::GravityComponent>())
        {
          auto *gravcmp = e->GetComponent<GravityComponent>();
          mc->velocity.y += gravcmp->gravityStrength * dt;
        }
        else
        {
          mc->velocity.y = Lerp(mc->velocity.y, mc->targetVelocity.y, mc->acceleration * dt);
        }

        // Apply Impulse
        if (e->HasComponent<ImpulseComponent>())
        {

          auto *impcmp = e->GetComponent<ImpulseComponent>();

          if (impcmp->IsActive())
          {
            // Decay Force
            impcmp->force = impcmp->force * exp(-impcmp->elapsedTime);

            // Scale Impulse
            Vector2 impulse = Vector2Scale(impcmp->direction, impcmp->force);

            // Apply
            Vector2Add(mc->velocity, impulse);

            // Clamp to 0
            if (impcmp->force < 5e-5)
            {
              impcmp->force = 0;
            }
            impcmp->elapsedTime += dt;
          }
          else
          {
            impcmp->direction = {.x = 0, .y = 0};
          }
        }

        if (fabs(mc->velocity.x) < 5e-5)
        {
          mc->velocity.x = 0;
        }

        if (fabs(mc->velocity.y) < 5e-5)
        {
          mc->velocity.y = 0;
        }

        bool hasBBox = e->HasComponent<BoundingBoxComponent>();

        // Update X pos
        if (hasBBox)
        {
          auto *abbcmp = e->GetComponent<BoundingBoxComponent>();
          abbcmp->lastPosition.x = transcmp->position.x;
        }
        transcmp->position.x = transcmp->position.x + (mc->velocity.x * mc->speed * dt);

        // Handle X Collision
        if (hasBBox)
        {
          HandleCollisions(e, 0, entitymanager);
        }

        // Update X pos
        if (hasBBox)
        {
          auto *abbcmp = e->GetComponent<BoundingBoxComponent>();
          abbcmp->lastPosition.y = transcmp->position.y;
        }

        if (e->HasComponent<GravityComponent>())
        {
          transcmp->position.y = transcmp->position.y + (mc->velocity.y * dt);
        }
        else
        {
          transcmp->position.y = transcmp->position.y + (mc->velocity.y * mc->speed * dt);
        }

        // Handle Y Collision
        if (hasBBox)
        {
          HandleCollisions(e, 1, entitymanager);
        }

        if (hasBBox)
        {
          auto *abbcmp = e->GetComponent<BoundingBoxComponent>();
        }
      }
    }
  }

  void MoveSystem::HandleCollisions(std::shared_ptr<Entity> &e, int axis, EntityManager *entityManager)
  {
    std::vector<std::shared_ptr<Entity>> entites = entityManager->Query<BoundingBoxComponent>();

    auto *abbcmp1 = e->GetComponent<BoundingBoxComponent>();
    auto *mvcmp1 = e->GetComponent<MoveComponent>();
    auto *transcmp1 = e->GetComponent<TransformComponent>();

    // Apply Positional Offset
    Vector2 currentRectPos = {
      transcmp1->position.x - abbcmp1->positionOffset.x,
      transcmp1->position.y - abbcmp1->positionOffset.y,
    };

    Vector2 lastRectPos = {
      abbcmp1->lastPosition.x - abbcmp1->positionOffset.x,
      abbcmp1->lastPosition.y - abbcmp1->positionOffset.y,
    };

    for (std::shared_ptr<Entity> &e2 : entites)
    {
      if (         //
        e != e2 && //
        abbcmp1->HasTypeFlag(BoundingBoxComponent::Type::COLLIDER) &&
        e2->GetComponent<BoundingBoxComponent>()->HasTypeFlag(BoundingBoxComponent::Type::COLLIDER) //
      )
      {
        auto *abbcmp2 = e2->GetComponent<BoundingBoxComponent>();
        auto *transcmp2 = e2->GetComponent<TransformComponent>();
        abbcmp2->lastPosition = transcmp2->position;

        if (                                                                                  //
          CheckCollisionRecs(                                                                 //
            {transcmp2->position.x, transcmp2->position.y, abbcmp2->size.x, abbcmp2->size.y}, //
            {currentRectPos.x, currentRectPos.y, abbcmp1->size.x, abbcmp1->size.y}            //
            )                                                                                 //
        )
        {
          if (axis == 0)
          {
            if (lastRectPos.x + abbcmp1->size.x <= transcmp2->position.x)
            {
              mvcmp1->velocity.x = 0;
              currentRectPos.x = transcmp2->position.x - abbcmp1->size.x;
            }
            else if (                                                  //
              lastRectPos.x >= transcmp2->position.x + abbcmp2->size.x //
            )
            {
              mvcmp1->velocity.x = 0;
              currentRectPos.x = transcmp2->position.x + abbcmp1->size.x;
            }
          }
          else
          {
            if (lastRectPos.y + abbcmp1->size.y <= transcmp2->position.y)
            {
              mvcmp1->velocity.y = 0;
              currentRectPos.y = transcmp2->position.y - abbcmp1->size.y;

              if (e->HasComponent<GravityComponent>())
              {
                auto *gravcmp = e->GetComponent<GravityComponent>();
                gravcmp->isJumping = false;
              }
            }
            else if (                                                  //
              lastRectPos.y >= transcmp2->position.y + abbcmp2->size.y //
            )
            {
              mvcmp1->velocity.y = 0;
              currentRectPos.y = transcmp2->position.y + abbcmp1->size.y;
            }
          }

          abbcmp1->lastPosition = transcmp1->position;
          transcmp1->position.x = currentRectPos.x + abbcmp1->positionOffset.x;
          transcmp1->position.y = currentRectPos.y + abbcmp1->positionOffset.y;
        }
      }
    }
  }
} // namespace Base
