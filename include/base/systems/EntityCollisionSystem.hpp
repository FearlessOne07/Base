#include "System.hpp"
#include "raylib.h"
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
    void Update(float dt, EntityManager *entityManager,const Scene *) override;
  };
} // namespace Base
