#pragma once
#include "base/Entity.hpp"
#include "base/Exports.hpp"
#include "base/System.hpp"
#include <memory>

namespace Base
{
  class BASEAPI MoveSystem : public System
  {
  public:
    void Update(float dt, EntityManager *entityManager) override;
    void HandleCollisions(std::shared_ptr<Entity> e, int axis, EntityManager *entityManager);
  };
} // namespace Base
