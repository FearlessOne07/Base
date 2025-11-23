#pragma once
#include "base/entities/Entity.hpp"
#include "base/systems/System.hpp"
#include <memory>

namespace Base
{
  class MoveSystem : public System
  {
  private:
    void ResolveBoxBox(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2, int axis);
    void HandleCollisions(std::shared_ptr<Entity> &e, int axis, Ref<EntityManager> entityManager);

  public:
    void Update(float dt, Ref<EntityManager> entityManager, std::shared_ptr<const Scene> scene) override;
  };
} // namespace Base
