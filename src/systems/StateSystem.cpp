#include "internal/systems/StateSystem.hpp"
#include "base/components/StateComponent.hpp"
#include <algorithm>

namespace Base
{
  void StateSystem::Update(float dt, Ref<EntityManager> entityManager, std::shared_ptr<Scene> currentScene)
  {
    auto entities_state = entityManager->Query<StateComponent>();

    for (auto &item : entities_state)
    {
      auto statecmp = item->item->GetComponent<StateComponent>();
      if (!statecmp->IsInitialized())
      {
        statecmp->SwitchState("", true);
      }

      auto &currentState = statecmp->GetCurrentState();
      bool transition = false;
      const std::vector<bool> &flags = currentState.transitionBlock.GetFlags();

      if (currentState.transitionBlock.GetEvaluationType() == TransitionEvaluationType::Or)
      {
        transition = std::any_of(flags.begin(), flags.end(), [](bool f) { return f; });
      }
      else if (currentState.transitionBlock.GetEvaluationType() == TransitionEvaluationType::And)
      {
        transition = std::all_of(flags.begin(), flags.end(), [](bool f) { return f; });
      }

      if (transition)
      {
        statecmp->SwitchState(currentState.transitionBlock.GetNextState());
        currentState.transitionBlock.ResetFlags();
      }
    }
  }
} // namespace Base
