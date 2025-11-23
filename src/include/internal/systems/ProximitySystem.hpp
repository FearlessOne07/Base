#pragma once

#include "base/systems/System.hpp"
namespace Base
{
  class ProximitySystem : public System
  {
  public:
    void Update(float dt, Ref<EntityManager> entityManager, std::shared_ptr<const Scene> scene) override;
  };
} // namespace Base
