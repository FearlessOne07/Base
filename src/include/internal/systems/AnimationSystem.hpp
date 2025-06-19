#include "base/systems/System.hpp"

namespace Base
{
  class AnimationSystem : public System
  {
  public:
    void Update(float dt, EntityManager *entityManager, const Scene *currentScene) override;
  };
} // namespace Base
