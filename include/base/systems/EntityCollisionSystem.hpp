#include "base/System.hpp"

namespace Base
{
  class EntityCollisionSystem : public System
  {
  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
