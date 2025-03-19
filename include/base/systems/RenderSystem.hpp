#pragma once
#include "base/System.hpp"

namespace Base
{
  class RenderSystem : public System
  {
  private:
    void Start() override;
    void Stop() override;

  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
