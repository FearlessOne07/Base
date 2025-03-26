#include "base/systems/EntityCollisionSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/EventBus.hpp"
#include "base/components/BoundingBoxComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/events/EntityCollisionEvent.hpp"
#include "raylib.h"
#include "raylib/raylib.h"
#include <memory>
#include <vector>

namespace Base
{
  void EntityCollisionSystem::Update(float dt, EntityManager *entityManager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entityManager->Query<Base::BoundingBoxComponent>();

    for (auto &e1 : entities)
    {
      for (auto &e2 : entities)
      {
        if ((e1 != e2) && (e1->IsAlive() && e2->IsAlive()))
        {
          if (e1->HasComponent<BoundingBoxComponent>() && e2->HasComponent<BoundingBoxComponent>())
          {
            auto *abb1 = e1->GetComponent<BoundingBoxComponent>();
            auto *trans1 = e1->GetComponent<TransformComponent>();
            auto *abb2 = e2->GetComponent<BoundingBoxComponent>();
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

            if ((CheckCollisionRecs(rect1, rect2)))
            {
              std::shared_ptr<EntityCollisionEvent> event = std::make_shared<EntityCollisionEvent>();

              if ( //
                (abb1->HasTypeFlag(BoundingBoxComponent::Type::HITBOX) &&
                 abb2->HasTypeFlag(BoundingBoxComponent::Type::HURTBOX)) //
              )
              {
                event->hittBoxEntity = e1;
                event->hurtBoxEntity = e2;
                EventBus::GetInstance()->Dispatch(event);
              }
              else if ( //
                (abb2->HasTypeFlag(BoundingBoxComponent::Type::HITBOX) &&
                 abb1->HasTypeFlag(BoundingBoxComponent::Type::HURTBOX)) //
              )
              {
                event->hittBoxEntity = e2;
                event->hurtBoxEntity = e1;
                EventBus::GetInstance()->Dispatch(event);
              }
            }
          }
        }
      }
    }
  }
} // namespace Base
