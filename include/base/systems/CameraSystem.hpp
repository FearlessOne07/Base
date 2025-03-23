#pragma once
#include "base/System.hpp"
#include "base/components/CameraComponent.hpp"

namespace Base
{
  class CameraSystem : public System
  {
  private:
    void SmoothFollow(float dt, CameraComponent *);
    void BasicFollow(float dt, CameraComponent *);

  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
