#include "base/systems/EntityCollisionSystem.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/entities/signals/EntityCollisionSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace Base
{
  void EntityCollisionSystem::Update(float dt, EntityManager *entityManager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entityManager->Query<Base::ColliderComponent>();

    for (auto &e1 : entities)
    {
      for (auto &e2 : entities)
      {
        if ((e1 != e2) && (e1->IsAlive() && e2->IsAlive()))
        {
          if (e1->HasComponent<ColliderComponent>() && e2->HasComponent<ColliderComponent>())
          {

            auto *abb1 = e1->GetComponent<ColliderComponent>();
            auto *abb2 = e2->GetComponent<ColliderComponent>();
            bool collision = false;
            Vector2 normal = {0, 0};

            if (abb1->shape == ColliderComponent::Shape::BOX && abb2->shape == ColliderComponent::Shape::BOX)
            {
              collision = BoxVsBoxCollision(e1, e2);
            }
            else if (abb1->shape == ColliderComponent::Shape::CIRCLE && abb2->shape == ColliderComponent::Shape::CIRCLE)
            {
              collision = CircleVsCircleCollision(e1, e2, normal);
            }
            else if (abb1->shape == ColliderComponent::Shape::CIRCLE && abb2->shape == ColliderComponent::Shape::BOX)
            {
              collision = CircleVsBoxCollision(e1, e2, normal);
            }
            else if (abb1->shape == ColliderComponent::Shape::BOX && abb2->shape == ColliderComponent::Shape::CIRCLE)
            {
              collision = CircleVsBoxCollision(e2, e1, normal);
            }

            if (collision)
            {
              std::shared_ptr<EntityCollisionSignal> event = std::make_shared<EntityCollisionSignal>();
              event->collisionNormal = normal;

              if ( //
                (abb1->HasTypeFlag(ColliderComponent::Type::HITBOX) &&
                 abb2->HasTypeFlag(ColliderComponent::Type::HURTBOX)) //
              )
              {
                event->hittBoxEntity = e1;
                event->hurtBoxEntity = e2;
                SignalBus::GetInstance()->BroadCastSignal(event);
              }
              else if ( //
                (abb2->HasTypeFlag(ColliderComponent::Type::HITBOX) &&
                 abb1->HasTypeFlag(ColliderComponent::Type::HURTBOX)) //
              )
              {
                event->hittBoxEntity = e2;
                event->hurtBoxEntity = e1;
                SignalBus::GetInstance()->BroadCastSignal(event);
              }
            }
          }
        }
      }
    }
  }

  bool EntityCollisionSystem::BoxVsBoxCollision(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2)
  {
    auto *abb1 = e1->GetComponent<ColliderComponent>();
    auto *abb2 = e2->GetComponent<ColliderComponent>();
    auto *trans1 = e1->GetComponent<TransformComponent>();
    auto *trans2 = e2->GetComponent<TransformComponent>();

    Rectangle rect1 = {
      trans1->position.x - abb1->positionOffset.x,
      trans1->position.y - abb1->positionOffset.y,
      abb1->size.x,
      abb1->size.y,
    };

    Rectangle rect2 = {
      trans2->position.x - abb2->positionOffset.x,
      trans2->position.y - abb2->positionOffset.y,
      abb2->size.x,
      abb2->size.y,
    };

    return CheckCollisionRecs(rect1, rect2);
  }

  bool EntityCollisionSystem::CircleVsCircleCollision(                           //
    std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2, Vector2 &outNormal //
  )
  {
    auto *abb1 = e1->GetComponent<ColliderComponent>();
    auto rbcmp1 = e1->GetComponent<RigidBodyComponent>();
    auto *abb2 = e2->GetComponent<ColliderComponent>();
    auto *trans1 = e1->GetComponent<TransformComponent>();
    auto *trans2 = e2->GetComponent<TransformComponent>();

    Vector2 position1 = trans1->position - abb1->positionOffset;
    Vector2 position2 = trans2->position - abb2->positionOffset;

    outNormal = Vector2Normalize(rbcmp1->velocity);
    return CheckCollisionCircles(position1, abb1->radius, position2, abb2->radius);
  }

  bool EntityCollisionSystem::CircleVsBoxCollision(                                               //
    std::shared_ptr<Entity> &circleEntity, std::shared_ptr<Entity> &boxEntity, Vector2 &outNormal //
  )
  {
    auto *colCircle = circleEntity->GetComponent<ColliderComponent>();
    auto *colBox = boxEntity->GetComponent<ColliderComponent>();
    auto *transCircle = circleEntity->GetComponent<TransformComponent>();
    auto *transBox = boxEntity->GetComponent<TransformComponent>();

    Vector2 rectBoundsX = {
      transBox->position.x - colBox->positionOffset.x,
      transBox->position.x + colBox->size.x - colBox->positionOffset.x,
    };
    Vector2 rectBoundsY = {
      transBox->position.y - colBox->positionOffset.y,
      transBox->position.y + colBox->size.y - colBox->positionOffset.y,
    };
    float closestX = std::clamp(transCircle->position.x, rectBoundsX.x, rectBoundsX.y);
    float closestY = std::clamp(transCircle->position.y, rectBoundsY.x, rectBoundsY.y);

    // Vector from closest point to circle center
    Vector2 difference = Vector2Subtract(transCircle->position, {closestX, closestY});
    float distanceSq = Vector2LengthSqr(difference);

    if (distanceSq < colCircle->radius * colCircle->radius)
    {
      float distance = sqrtf(distanceSq);
      if (distance != 0)
        outNormal = Vector2Scale(difference, 1.0f / distance); // Normalize
      else
        outNormal = {.x = 0.0f, .y = -1.0f}; // Arbitrary normal if center is exactly at the corner
      return true;
    }
    return false;
  }
} // namespace Base
