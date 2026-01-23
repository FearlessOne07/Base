#include "base/systems/System.hpp"
#include <memory>

namespace Base
{
  class EntityCollisionSystem : public System
  {
  private:
    bool BoxVsBoxCollision(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2);
    bool CircleVsCircleCollision(                                                  //
      std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2, Vector2 &outNormal //
    );
    bool CircleVsBoxCollision(                                                                      //
      std::shared_ptr<Entity> &circleEntity, std::shared_ptr<Entity> &boxEntity, Vector2 &outNormal //
    );

  public:
    void Update(float dt, Ref<EntityManager> entityManager, std::shared_ptr<Scene> scene) override;
  };
} // namespace Base
