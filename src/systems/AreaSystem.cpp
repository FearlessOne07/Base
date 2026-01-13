#include "internal/systems/AreaSystem.hpp"
#include "base/components/AreaComponent.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/StateComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"

namespace Base
{

  static bool FitsEntirelyRect(Rectangle rect, Rectangle container)
  {
    return rect.GetPosition().x >= container.GetPosition().x && rect.GetPosition().y >= container.GetPosition().y &&
           rect.GetPosition().x + rect.GetSize().x <= container.GetPosition().x + container.GetSize().x &&
           rect.GetPosition().y + rect.GetSize().y <= container.GetPosition().y + container.GetSize().y;
  }

  static bool FitsEntirelyCircle(Circle circle, Rectangle container)
  {
    return (circle.GetPosition().x - circle.GetRadius()) >= container.GetPosition().x &&
           (circle.GetPosition().y - circle.GetRadius()) >= container.GetPosition().y &&
           (circle.GetPosition().x + circle.GetRadius()) <= (container.GetPosition().x + container.GetSize().x) &&
           (circle.GetPosition().y + circle.GetRadius()) <= (container.GetPosition().y + container.GetSize().y);
  }

  void AreaSystem::Update(float dt, Ref<EntityManager> entityManager, std::shared_ptr<const Scene> currentScene)
  {
    auto entities_arentry = entityManager->Query<StateComponent, AreaEntry>();
    for (auto &item : entities_arentry)
    {
      auto arentry = item->item->GetComponent<AreaEntry>();
      auto colcmp = item->item->GetComponent<ColliderComponent>();
      auto transcmp = item->item->GetComponent<TransformComponent>();

      if (colcmp->shape == ColliderComponent::Shape::Circle)
      {
        if (FitsEntirelyCircle({transcmp->position, colcmp->radius}, arentry->GetArea()))
        {
          auto statecmp = item->item->GetComponent<StateComponent>();
          statecmp->GetCurrentState().transitionBlock.SetIndex(arentry->GetBlockIndex());
        }
      }
      else if (colcmp->shape == ColliderComponent::Shape::Box)
      {
        if ( //
          FitsEntirelyRect({transcmp->position, colcmp->size}, arentry->GetArea()))
        {
          auto statecmp = item->item->GetComponent<StateComponent>();
          statecmp->GetCurrentState().transitionBlock.SetIndex(arentry->GetBlockIndex());
        }
      }
    }

    auto entities_arexit = entityManager->Query<StateComponent, AreaExit>();
    for (auto &item : entities_arexit)
    {
      auto arexit = item->item->GetComponent<AreaExit>();
      auto colcmp = item->item->GetComponent<ColliderComponent>();
      auto transcmp = item->item->GetComponent<TransformComponent>();

      if (colcmp->shape == ColliderComponent::Shape::Circle)
      {
        if (!FitsEntirelyCircle({transcmp->position, colcmp->radius}, arexit->GetArea()))
        {
          auto statecmp = item->item->GetComponent<StateComponent>();
          statecmp->GetCurrentState().transitionBlock.SetIndex(arexit->GetBlockIndex());
        }
      }
      else if (colcmp->shape == ColliderComponent::Shape::Box)
      {
        if ( //
          !FitsEntirelyRect(
            {
              transcmp->position,
              colcmp->size,
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
