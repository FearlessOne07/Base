#pragma once
#include "System.hpp"
#include "base/entities/Entity.hpp"
#include <memory>

namespace Base
{
  class MoveSystem : public System
  {
  private:
    void ResolveBoxBox(std::shared_ptr<Entity> &e1, std::shared_ptr<Entity> &e2, int axis);
    void HandleCollisions(std::shared_ptr<Entity> &e, int axis, EntityManager *entityManager);

  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
