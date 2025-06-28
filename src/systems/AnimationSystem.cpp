#include "internal/systems/AnimationSystem.hpp"
#include "base/components/AnimationComponent.hpp"
#include "base/components/SpriteComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/util/Exception.hpp"
#include <sstream>

namespace Base
{
  void AnimationSystem::Update(float dt, EntityManager *entityManager, const Scene *currentScene)
  {
    auto entities_animcmp = entityManager->Query<AnimationComponent>();

    for (auto &e : entities_animcmp)
    {
      if (e->item->HasComponent<SpriteComponent>())
      {
        auto *sprtcmp = e->item->GetComponent<SpriteComponent>();
        auto animcomp = e->item->GetComponent<AnimationComponent>();

        if (!animcomp->IsDone())
        {
          sprtcmp->SetSourceIndex({static_cast<float>(animcomp->GetCurrentFrame()), sprtcmp->GetSourceIndex().y});
        }
        animcomp->Advance(dt);
      }
      else
      {
        std::stringstream err;
        err << "Entity " << e->item->GetID() << " has an AnimComponent but no texture component";
        THROW_BASE_RUNTIME_ERROR(err.str());
      }
    }
  }
} // namespace Base
