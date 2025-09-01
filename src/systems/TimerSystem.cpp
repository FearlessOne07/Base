
#include "internal/systems/TimerSystem.hpp"
#include "base/components/StateComponent.hpp"
#include "base/components/TimerComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/scenes/Scene.hpp"
namespace Base
{
  void TimerSystem::Update(float dt, EntityManager *entityManager, const Scene *currentScene)
  {
    auto entities_timercmp = entityManager->Query<TimerComponent>();

    for (auto &e : entities_timercmp)
    {
      auto timecmp = e->item->GetComponent<TimerComponent>();

      if (timecmp->GetElapsedTime() < timecmp->GetDuration())
      {
        timecmp->Advance(dt);
      }
      else
      {
        timecmp->ResetTimer();
        auto statecmp = e->item->GetComponent<StateComponent>();
        statecmp->GetCurrentState().transitionBlock.SetIndex(timecmp->GetBlockIndex());
      }
    }
  }
} // namespace Base
