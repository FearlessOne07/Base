#include "internal/systems/AnimationSystem.hpp"
#include "base/components/AnimationComponent.hpp"
#include "base/components/TextureComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/util/Exception.hpp"
#include <sstream>
#include <vector>

namespace Base
{
  void AnimationSystem::Update(float dt, EntityManager *entityManager, const Scene *currentScene)
  {
    std::vector<std::shared_ptr<Entity>> entities_animcmp = entityManager->Query<AnimationComponent>();

    for (auto &e : entities_animcmp)
    {
      if (e->HasComponent<TextureComponent>())
      {
        auto *texcmp = e->GetComponent<TextureComponent>();
        auto animcomp = e->GetComponent<AnimationComponent>();

        if (!animcomp->IsDone())
        {
          texcmp->source.x += (animcomp->GetCurrentFrame() * texcmp->source.width);
        }
        animcomp->Advance(dt);
      }
      else
      {
        std::stringstream err;
        err << "Entity " << e->GetID() << " has an AnimComponent but no texture component";
        THROW_BASE_RUNTIME_ERROR(err.str());
      }
    }
  }
} // namespace Base
