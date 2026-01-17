#include "internal/systems/EntityCollisionSystem.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/entities/signals/EntityCollisionSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Circle.hpp"
#include "internal/utils/Collisions.hpp"
#include <algorithm>
#include <memory>

namespace Base
{
  void EntityCollisionSystem::Update( //
    float dt, Ref<EntityManager> entityManager,
    std::shared_ptr<Scene> currentScene //
  )
  {
    auto entities = entityManager->Query<Base::ColliderComponent>();

    for (auto &item1 : entities)
    {
      auto e1 = item1->item;
      auto colcmp1 = e1->GetComponent<ColliderComponent>();
      std::list<std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator> nearby;
      if (colcmp1->shape == ColliderComponent::Shape::Circle)
      {
        nearby = entityManager->QueryArea(Circle{e1->GetComponent<TransformComponent>()->position, colcmp1->radius});
      }
      else if (colcmp1->shape == ColliderComponent::Shape::Box)
      {
        nearby = entityManager->QueryArea(Rectangle{
          e1->GetComponent<TransformComponent>()->position - colcmp1->positionOffset,
          colcmp1->size,
        });
      }

      for (auto &item2 : nearby)
      {
        auto e2 = item2->item;
        if ((e1 != e2) && (e1->IsAlive() && e2->IsAlive()))
        {
          if (e1->HasComponent<ColliderComponent>() && e2->HasComponent<ColliderComponent>())
          {
            auto abb1 = e1->GetComponent<ColliderComponent>();
            auto abb2 = e2->GetComponent<ColliderComponent>();
            bool collision = false;
            Vector2 normal = {0, 0};

            if (abb1->shape == ColliderComponent::Shape::Box && abb2->shape == ColliderComponent::Shape::Box)
            {
              collision = BoxVsBoxCollision(e1, e2);
            }
            else if (abb1->shape == ColliderComponent::Shape::Circle && abb2->shape == ColliderComponent::Shape::Circle)
            {
              collision = CircleVsCircleCollision(e1, e2, normal);
            }
            else if (abb1->shape == ColliderComponent::Shape::Circle && abb2->shape == ColliderComponent::Shape::Box)
            {
              collision = CircleVsBoxCollision(e1, e2, normal);
            }
            else if (abb1->shape == ColliderComponent::Shape::Box && abb2->shape == ColliderComponent::Shape::Circle)
            {
              collision = CircleVsBoxCollision(e2, e1, normal);
            }

            if (collision)
            {
              std::shared_ptr<EntityCollisionSignal> event = std::make_shared<EntityCollisionSignal>();
              event->collisionNormal = normal;
              event->hittBoxEntity = e1;
              event->hurtBoxEntity = e2;
              SignalBus::GetInstance()->BroadCastSignal(event);
            }
          }
        }
      }
    }
  }

  bool EntityCollisionSystem::BoxVsBoxCollision(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2)
  {
    auto abb1 = e1->GetComponent<ColliderComponent>();
    auto abb2 = e2->GetComponent<ColliderComponent>();
    auto trans1 = e1->GetComponent<TransformComponent>();
    auto trans2 = e2->GetComponent<TransformComponent>();

    Rectangle rect1 = {
      trans1->position - abb1->positionOffset,
      abb1->size,
    };

    Rectangle rect2 = {
      trans2->position - abb2->positionOffset,
      abb2->size,
    };

    return CheckCollisionRecs(rect1, rect2);
  }

  bool EntityCollisionSystem::CircleVsCircleCollision(                           //
    std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2, Vector2 &outNormal //
  )
  {
    auto abb1 = e1->GetComponent<ColliderComponent>();
    auto abb2 = e2->GetComponent<ColliderComponent>();
    auto trans1 = e1->GetComponent<TransformComponent>();
    auto trans2 = e2->GetComponent<TransformComponent>();

    Vector2 position1 = trans1->position - abb1->positionOffset;
    Vector2 position2 = trans2->position - abb2->positionOffset;

    return CheckCollisionCircles({position1, abb1->radius}, {position2, abb2->radius});
  }

  bool EntityCollisionSystem::CircleVsBoxCollision(                                               //
    std::shared_ptr<Entity> &circleEntity, std::shared_ptr<Entity> &boxEntity, Vector2 &outNormal //
  )
  {
    auto colCircle = circleEntity->GetComponent<ColliderComponent>();
    auto colBox = boxEntity->GetComponent<ColliderComponent>();
    auto transCircle = circleEntity->GetComponent<TransformComponent>();
    auto transBox = boxEntity->GetComponent<TransformComponent>();

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
    Vector2 difference = transCircle->position - Vector2{closestX, closestY};
    float distance = glm::length(difference);

    if (distance < colCircle->radius)
    {
      if (distance != 0)
        outNormal = difference * (1.0f / distance); // Normalize
      else
        outNormal = Vector2{0.0f, -1.0f}; // Arbitrary normal if center is exactly at the corner
      return true;
    }
    return false;
  }
} // namespace Base
