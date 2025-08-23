#include "internal/systems/AreaSystem.hpp"
#include "base/components/AreaComponent.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/StateComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "raylib.h"

namespace Base
{

  static bool FitsEntirelyRect(Rectangle rect, Rectangle container)
  {
    return rect.x >= container.x && rect.y >= container.y && rect.x + rect.width <= container.x + container.width &&
           rect.y + rect.height <= container.y + container.height;
  }

  static bool FitsEntirelyCircle(Circle circle, Rectangle container)
  {
    return (circle.position.x - circle.radius) >= container.x && (circle.position.y - circle.radius) >= container.y &&
           (circle.position.x + circle.radius) <= (container.x + container.width) &&
           (circle.position.y + circle.radius) <= (container.y + container.height);
  }

  void AreaSystem::Update(float dt, EntityManager *entitymanager, const Scene *currentScene)
  {
    auto entities_arentry = entitymanager->Query<StateComponent, AreaEntry>();
    for (auto &item : entities_arentry)
    {
      auto arentry = item->item->GetComponent<AreaEntry>();
      auto colcmp = item->item->GetComponent<ColliderComponent>();
      auto transcmp = item->item->GetComponent<TransformComponent>();

      if (colcmp->shape == ColliderComponent::Shape::CIRCLE)
      {
        if (FitsEntirelyCircle({transcmp->position, colcmp->radius}, arentry->GetArea()))
        {
          auto statecmp = item->item->GetComponent<StateComponent>();
          statecmp->GetCurrentState().transitionBlock.SetIndex(arentry->GetBlockIndex());
        }
      }
      else if (colcmp->shape == ColliderComponent::Shape::BOX)
      {
        if ( //
          FitsEntirelyRect(
            {
              transcmp->position.x,
              transcmp->position.y,
              colcmp->size.x,
              colcmp->size.y,
            },
            arentry->GetArea()) //
        )
        {
          auto statecmp = item->item->GetComponent<StateComponent>();
          statecmp->GetCurrentState().transitionBlock.SetIndex(arentry->GetBlockIndex());
        }
      }
    }

    auto entities_arexit = entitymanager->Query<StateComponent, AreaExit>();
    for (auto &item : entities_arexit)
    {
      auto arexit = item->item->GetComponent<AreaExit>();
      auto colcmp = item->item->GetComponent<ColliderComponent>();
      auto transcmp = item->item->GetComponent<TransformComponent>();

      if (colcmp->shape == ColliderComponent::Shape::CIRCLE)
      {
        if (!FitsEntirelyCircle({transcmp->position, colcmp->radius}, arexit->GetArea()))
        {
          auto statecmp = item->item->GetComponent<StateComponent>();
          statecmp->GetCurrentState().transitionBlock.SetIndex(arexit->GetBlockIndex());
        }
      }
      else if (colcmp->shape == ColliderComponent::Shape::BOX)
      {
        if ( //
          !FitsEntirelyRect(
            {
              transcmp->position.x,
              transcmp->position.y,
              colcmp->size.x,
              colcmp->size.y,
            },
            arexit->GetArea()) //
        )
        {
          auto statecmp = item->item->GetComponent<StateComponent>();
          statecmp->GetCurrentState().transitionBlock.SetIndex(arexit->GetBlockIndex());
        }
      }
    }
  }
} // namespace Base
