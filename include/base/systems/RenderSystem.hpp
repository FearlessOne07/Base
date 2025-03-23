#pragma once
#include "base/System.hpp"

namespace Base
{
  class RenderSystem : public System
  {
  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
