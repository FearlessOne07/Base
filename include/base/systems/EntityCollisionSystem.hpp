#include "base/System.hpp"
#include <memory>

namespace Base
{
  class EntityCollisionSystem : public System
  {
  private:
    void BoxVsBoxCollision(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2);

  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
