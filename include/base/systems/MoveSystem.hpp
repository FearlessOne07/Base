#pragma once
#include "base/Exports.hpp"
#include "base/System.hpp"

namespace Base
{
  class BASEAPI MoveSystem : public System
  {
  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
