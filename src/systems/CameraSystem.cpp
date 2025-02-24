#include "base/systems/CameraSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/components/ABBComponent.hpp"
#include "base/components/CameraComponent.hpp"
#include "raylib.h"
#include "raymath.h"
#include <memory>
#include <vector>

namespace Base
{
  void CameraSystem::Update(float dt, EntityManager *entityManager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entityManager->Query<CameraComponent>();

    for (auto &e : entities)
    {
      CameraComponent *camcmp = e->GetComponent<CameraComponent>();

      if (e->HasComponent<ABBComponent>())
      {
        ABBComponent *abbcmp = e->GetComponent<ABBComponent>();
      }
      switch (camcmp->cameraMode)
      {
      case CameraMode::BASIC_FOLLOW:
        BasicFollow(dt, camcmp);
      case CameraMode::SMOOTH_FOLLOW:
        SmoothFollow(dt, camcmp);
      };
    }
  }
  void CameraSystem::BasicFollow(float dt, CameraComponent *camcmp)
  {
    const RenderContext *rd = RenderContextSingleton::GetInstance();
    rd->camera.target = camcmp->target;
  }
  void CameraSystem::SmoothFollow(float dt, CameraComponent *camcmp)
  {
    const RenderContext *rd = RenderContextSingleton::GetInstance();
    rd->camera.target = Vector2Lerp(rd->camera.target, camcmp->target, 2 * dt);
  }
} // namespace Base
