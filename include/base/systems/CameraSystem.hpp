#pragma once
#include "base/System.hpp"
#include "base/components/CameraComponent.hpp"

namespace Base
{
  class BASEAPI CameraSystem : public System
  {
    void Update(float dt, EntityManager *entityManager) override;

  public:
    void SmoothFollow(float dt, CameraComponent *);
    void BasicFollow(float dt, CameraComponent *);
  };
} // namespace Base
