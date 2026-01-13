#include "internal/systems/ProximitySystem.hpp"
#include "base/components/ProximityComponent.hpp"
#include "base/components/StateComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"

namespace Base
{

  void ProximitySystem::Update(float dt, Ref<EntityManager> entityManager, std::shared_ptr<const Scene> currentScene)
  {
    auto entities_proxentry = entityManager->Query<StateComponent, ProximityEntry>();
    for (auto &item : entities_proxentry)
    {
      auto proxentry = item->item->GetComponent<ProximityEntry>();
      auto radiusEntity = entityManager->GetEntity(proxentry->GetRadiusEntity());

      Vector2 radiusEntityPosition = radiusEntity->GetComponent<TransformComponent>()->position;

      auto transcmp = item->item->GetComponent<TransformComponent>();
      float distance = glm::distance(radiusEntityPosition, transcmp->position);

      if (distance * distance <= proxentry->GetRadiusSq())
      {
        auto statecmp = item->item->GetComponent<StateComponent>();
        statecmp->GetCurrentState().transitionBlock.SetIndex(proxentry->GetBlockIndex());
      }
    }

    auto entities_proxexit = entityManager->Query<StateComponent, ProximityExit>();
    for (auto &item : entities_proxexit)
    {
      auto proxexit = item->item->GetComponent<ProximityExit>();
      auto radiusEntity = entityManager->GetEntity(proxexit->GetRadiusEntity());

      Vector2 radiusEntityPosition = radiusEntity->GetComponent<TransformComponent>()->position;

      auto transcmp = item->item->GetComponent<TransformComponent>();
      float distance = glm::distance(radiusEntityPosition, transcmp->position);

      if (distance * distance >= proxexit->GetRadiusSq())
      {
        auto statecmp = item->item->GetComponent<StateComponent>();
        statecmp->GetCurrentState().transitionBlock.SetIndex(proxexit->GetBlockIndex());
      }
    }
  }
} // namespace Base
