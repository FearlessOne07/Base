#include "base/System.hpp"
#include <memory>

namespace Base
{
  class EntityCollisionSystem : public System
  {
  private:
    bool BoxVsBoxCollision(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2);
    bool CircleVsCircleCollision(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2);

  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
