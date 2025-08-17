#pragma once

#include "base/systems/System.hpp"
namespace Base
{
  class ProximitySystem : public System
  {
  public:
    void Update(float dt, EntityManager *entityManager, const Scene *) override;
  };
} // namespace Base
