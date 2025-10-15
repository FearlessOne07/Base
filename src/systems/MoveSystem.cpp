#include "internal/systems/MoveSystem.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/GravityComponent.hpp"
#include "base/components/ImpulseComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/Entity.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/util/Circle.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <memory>

namespace Base
{
  void MoveSystem::Update(float dt, EntityManager *entitymanager, const Scene *currentScene)
  {
    auto entities = entitymanager->Query<RigidBodyComponent, MoveComponent>();
    for (auto &item : entities)
    {
      auto e = item->item;
      if (e)
      {
        auto rbcmp = e->GetComponent<RigidBodyComponent>();
        auto mvcmp = e->GetComponent<MoveComponent>();
        auto transcmp = e->GetComponent<TransformComponent>();

        if (!rbcmp->isKinematic)
        {
          if (rbcmp->mass > 0)
          {
            Vector2 totalForce = {0, 0};

            Vector2 direction = Vector2Normalize(rbcmp->direction);

            // Driving force
            if (Vector2Length(direction) > 0)
            {
              totalForce += direction * mvcmp->driveForce;
            }
            else if (Vector2Length(rbcmp->velocity) > 0)
            {
              // Braking force opposite to velocity
              Vector2 velDir = Vector2Normalize(rbcmp->velocity);
              totalForce += velDir * -mvcmp->brakeForce;
            }

            // Drag (air resistance proportional to velocity)
            totalForce += rbcmp->velocity * -(rbcmp->drag);

            Vector2 acceleration = totalForce / rbcmp->mass;
            rbcmp->velocity += acceleration * dt;

            if (e->HasComponent<ImpulseComponent>())
            {
              auto impcmp = e->GetComponent<ImpulseComponent>();
              if (impcmp->force > 0)
              {
                Vector2 impDirection = Vector2Normalize(impcmp->direction);
                float impulse = impcmp->force;
                rbcmp->velocity += impDirection * (impulse / rbcmp->mass);

                // clear after applying
                impcmp->force = 0;
                impcmp->direction = {0, 0};
              }
            }
          }
        }
        else
        {
          Vector2 direction = Vector2Normalize(rbcmp->direction);
          rbcmp->velocity = direction * mvcmp->speed;
        }

        if (abs(rbcmp->velocity.x) < 5e-10)
        {
          rbcmp->velocity.x = 0;
        }
        if (abs(rbcmp->velocity.y) < 5e-10)
        {
          rbcmp->velocity.y = 0;
        }

        // Update Positions
        transcmp->position.x += rbcmp->velocity.x * dt;
        HandleCollisions(e, 0, entitymanager);

        transcmp->position.y += rbcmp->velocity.y * dt;
        HandleCollisions(e, 1, entitymanager);

        entitymanager->UpdateEntity(item);
      }
    }
  }

  void MoveSystem::HandleCollisions(std::shared_ptr<Entity> &e, int axis, EntityManager *entityManager)
  {
    auto mvcmp1 = e->GetComponent<MoveComponent>();
    auto abbcmp1 = e->GetComponent<ColliderComponent>();
    auto transcmp1 = e->GetComponent<TransformComponent>();
    auto rbcmp1 = e->GetComponent<RigidBodyComponent>();
    std::list<std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator> entites;

    if (abbcmp1->shape == ColliderComponent::Shape::BOX)
    {
      entites = entityManager->QueryArea(
        Rectangle{transcmp1->position.x, transcmp1->position.y, abbcmp1->size.x, abbcmp1->size.y});
    }
    else if (abbcmp1->shape == ColliderComponent::Shape::CIRCLE)
    {
      entites = entityManager->QueryArea(Circle(transcmp1->position, abbcmp1->radius));
    }

    for (auto &item : entites)
    {
      auto e2 = item->item;
      if (e != e2)
      {
        auto abbcmp2 = e2->GetComponent<ColliderComponent>();

        if (abbcmp1->shape == ColliderComponent::Shape::BOX && abbcmp2->shape == ColliderComponent::Shape::BOX)
        {
          ResolveBoxBox(e, e2, axis);
        }
      }
    }
  }

  void MoveSystem::ResolveBoxBox(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2, int axis)
  {
    // e1
    auto abbcmp1 = e1->GetComponent<ColliderComponent>();
    auto mvcmp1 = e1->GetComponent<MoveComponent>();
    auto transcmp1 = e1->GetComponent<TransformComponent>();
    auto rbcmp1 = e1->GetComponent<RigidBodyComponent>();

    // e2
    auto abbcmp2 = e2->GetComponent<ColliderComponent>();
    auto transcmp2 = e2->GetComponent<TransformComponent>();
    auto rbcmp2 = e2->GetComponent<RigidBodyComponent>();
    abbcmp2->lastPosition = transcmp2->position;

    // Apply Positional Offset
    Vector2 currentRectPos = {
      transcmp1->position.x - abbcmp1->positionOffset.x,
      transcmp1->position.y - abbcmp1->positionOffset.y,
    };

    Vector2 lastRectPos = {
      abbcmp1->lastPosition.x - abbcmp1->positionOffset.x,
      abbcmp1->lastPosition.y - abbcmp1->positionOffset.y,
    };

    Vector2 e2Position = {
      transcmp2->position.x - abbcmp2->positionOffset.x,
      transcmp2->position.y - abbcmp2->positionOffset.y,
    };

    if (                                                                       //
      CheckCollisionRecs(                                                      //
        {e2Position.x, e2Position.y, abbcmp2->size.x, abbcmp2->size.y},        //
        {currentRectPos.x, currentRectPos.y, abbcmp1->size.x, abbcmp1->size.y} //
        )                                                                      //
    )
    {
      if (axis == 0)
      {
        if (lastRectPos.x + abbcmp1->size.x <= e2Position.x)
        {
          rbcmp1->velocity.x = 0;
          currentRectPos.x = e2Position.x - abbcmp1->size.x;
        }
        else if (                                         //
          lastRectPos.x >= e2Position.x + abbcmp2->size.x //
        )
        {
          rbcmp1->velocity.x = 0;
          currentRectPos.x = e2Position.x + abbcmp1->size.x;
        }
      }
      else
      {
        if (lastRectPos.y + abbcmp1->size.y <= e2Position.y)
        {
          rbcmp1->velocity.y = 0;
          currentRectPos.y = e2Position.y - abbcmp1->size.y;

          if (e1->HasComponent<GravityComponent>())
          {
            auto gravcmp = e1->GetComponent<GravityComponent>();
            gravcmp->isJumping = false;
          }
        }
        else if (                                         //
          lastRectPos.y >= e2Position.y + abbcmp2->size.y //
        )
        {
          rbcmp1->velocity.y = 0;
          currentRectPos.y = e2Position.y + abbcmp1->size.y;
        }
      }

      abbcmp1->lastPosition = transcmp1->position;
      transcmp1->position.x = currentRectPos.x + abbcmp1->positionOffset.x;
      transcmp1->position.y = currentRectPos.y + abbcmp1->positionOffset.y;
    }
  }
} // namespace Base
