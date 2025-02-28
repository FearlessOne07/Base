#include "base/systems/CameraSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/components/ABBComponent.hpp"
#include "base/components/CameraComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include <memory>
#include <vector>

namespace Base
{
  void CameraSystem::Update(float dt, EntityManager *entityManager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entityManager->Query<CameraComponent>();

    for (std::shared_ptr<Entity> &e : entities)
    {
      auto *camcmp = e->GetComponent<CameraComponent>();

      if (e->HasComponent<ABBComponent>())
      {
        auto *abbcmp = e->GetComponent<ABBComponent>();
        camcmp->target.x = abbcmp->boundingBox.x + (abbcmp->boundingBox.width / 2);
        camcmp->target.y = abbcmp->boundingBox.y + (abbcmp->boundingBox.height / 2);
      }
      else if (e->HasComponent<ShapeComponent>())
      {
        auto *transcmp = e->GetComponent<TransformComponent>();
        camcmp->target = transcmp->position;
      }
      switch (camcmp->cameraMode)
      {
      case CameraMode::BASIC_FOLLOW:
        BasicFollow(dt, camcmp);
        break;
      case CameraMode::SMOOTH_FOLLOW:
        SmoothFollow(dt, camcmp);
        break;
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
