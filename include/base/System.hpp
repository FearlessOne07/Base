#pragma once
#include "base/Entity.hpp"

namespace Base
{
  class EntityManager;
  class BASEAPI System
  {
  public:
    virtual ~System();
    virtual void Update(float dt, EntityManager *entityManager) = 0;
  };
} // namespace Base
