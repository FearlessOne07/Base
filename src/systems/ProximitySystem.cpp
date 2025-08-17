#include "internal/systems/ProximitySystem.hpp"
#include "base/components/ProximityComponent.hpp"
#include "base/components/StateComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "raymath.h"

namespace Base
{

  void ProximitySystem::Update(float dt, EntityManager *entitymanager, const Scene *currentScene)
  {
    // Shape Component
    auto entities_proxentry = entitymanager->Query<StateComponent, ProximityEntry>();
    for (auto &item : entities_proxentry)
    {
      auto proxentry = item->item->GetComponent<ProximityEntry>();
      auto radiusEntity = entitymanager->GetEntity(proxentry->GetRadiusEntity());

      Vector2 radiusEntityPosition = radiusEntity->GetComponent<TransformComponent>()->position;

      auto transcmp = item->item->GetComponent<TransformComponent>();
      float distance = Vector2DistanceSqr(radiusEntityPosition, transcmp->position);

      if (distance <= proxentry->GetRadiusSq())
      {
        auto statecmp = item->item->GetComponent<StateComponent>();
        statecmp->GetCurrentState().transitionBlock.SetIndex(proxentry->GetBlockIndex());
      }
    }

    // Shape Component
    auto entities_proxexit = entitymanager->Query<StateComponent, ProximityExit>();
    for (auto &item : entities_proxexit)
    {
      auto proxexit = item->item->GetComponent<ProximityExit>();
      auto radiusEntity = entitymanager->GetEntity(proxexit->GetRadiusEntity());

      Vector2 radiusEntityPosition = radiusEntity->GetComponent<TransformComponent>()->position;

      auto transcmp = item->item->GetComponent<TransformComponent>();
      float distance = Vector2DistanceSqr(radiusEntityPosition, transcmp->position);

      if (distance >= proxexit->GetRadiusSq())
      {
        auto statecmp = item->item->GetComponent<StateComponent>();
        statecmp->GetCurrentState().transitionBlock.SetIndex(proxexit->GetBlockIndex());
      }
    }
  }
} // namespace Base
