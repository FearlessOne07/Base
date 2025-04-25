#include "base/systems/MoveSystem.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/GravityComponent.hpp"
#include "base/components/ImpulseComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
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
    std::vector<std::shared_ptr<Entity>> entities = entitymanager->Query<RigidBodyComponent>();

    for (std::shared_ptr<Entity> &e : entities)
    {
      if (e)
      {
        auto *rbcmp = e->GetComponent<RigidBodyComponent>();
        auto *mvcmp = e->GetComponent<MoveComponent>();
        auto transcmp = e->GetComponent<TransformComponent>();

        if (!rbcmp->isKinematic)
        {
          Vector2 direction = Vector2Normalize(rbcmp->direction);

          Vector2 braking = {0, 0};
          Vector2 driving = {0, 0};
          Vector2 drag = {0, 0};

          // Apply Braking force if we arent moving
          if (Vector2Length(direction) > 0)
          {
            driving = direction * mvcmp->driveForce;
          }
          else if (Vector2Length(direction) == 0)
          {
            Vector2 velDir = Vector2Normalize(rbcmp->velocity);
            braking = velDir * -mvcmp->brakeForce;
          }

          // Drag
          drag = rbcmp->velocity * -(rbcmp->drag / rbcmp->mass);

          if (rbcmp->mass > 0)
          {
            rbcmp->velocity += ((driving + braking + drag) / rbcmp->mass) * dt;

            if (e->HasComponent<ImpulseComponent>())
            {
              auto *impcmp = e->GetComponent<ImpulseComponent>();
              if (impcmp->force > 0)
              {
                Vector2 impDirection = Vector2Normalize(impcmp->direction);
                rbcmp->velocity = {.x = 0, .y = 0};
                rbcmp->velocity += (impDirection) * (impcmp->force / rbcmp->mass);
                impcmp->force = 0;
                impcmp->direction = {.x = 0, .y = 0};
              }
            }
          }
        }
        else if (rbcmp->isKinematic)
        {
          Vector2 direction = Vector2Normalize(rbcmp->direction);
          rbcmp->velocity = direction * rbcmp->speed * dt;
        }

        if (abs(rbcmp->velocity.x) < 5e-10)
        {
          rbcmp->velocity.x = 0;
        }
        if ((rbcmp->velocity.y) < 5e-10)
        {
          rbcmp->velocity.y = 0;
        }
        // Update Positions
        transcmp->position += rbcmp->velocity * dt;
      }
    }
  }

  void MoveSystem::HandleCollisions(std::shared_ptr<Entity> &e, int axis, EntityManager *entityManager)
  {
    std::vector<std::shared_ptr<Entity>> entites = entityManager->Query<ColliderComponent>();

    auto *abbcmp1 = e->GetComponent<ColliderComponent>();
    auto *mvcmp1 = e->GetComponent<MoveComponent>();
    auto *transcmp1 = e->GetComponent<TransformComponent>();
    auto *rbcmp1 = e->GetComponent<RigidBodyComponent>();

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
        abbcmp1->HasTypeFlag(ColliderComponent::Type::COLLIDER) &&
        e2->GetComponent<ColliderComponent>()->HasTypeFlag(ColliderComponent::Type::COLLIDER) //
      )
      {
        auto *abbcmp2 = e2->GetComponent<ColliderComponent>();
        auto *transcmp2 = e2->GetComponent<TransformComponent>();
        auto *rbcmp2 = e2->GetComponent<RigidBodyComponent>();

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
              rbcmp1->velocity.x = 0;
              currentRectPos.x = transcmp2->position.x - abbcmp1->size.x;
            }
            else if (                                                  //
              lastRectPos.x >= transcmp2->position.x + abbcmp2->size.x //
            )
            {
              rbcmp1->velocity.x = 0;
              currentRectPos.x = transcmp2->position.x + abbcmp1->size.x;
            }
          }
          else
          {
            if (lastRectPos.y + abbcmp1->size.y <= transcmp2->position.y)
            {
              rbcmp1->velocity.y = 0;
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
              rbcmp1->velocity.y = 0;
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
