#pragma once
#include "System.hpp"

namespace Base
{
  class RenderSystem : public System
  {
  public:
    void Update(float dt, EntityManager *entityManager,const Scene *) override;
  };
} // namespace Base
