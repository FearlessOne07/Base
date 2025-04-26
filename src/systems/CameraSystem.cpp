#include "base/systems/CameraSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/components/CameraComponent.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
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
      auto *transcmp = e->GetComponent<TransformComponent>();

      if (e->HasComponent<ColliderComponent>())
      {
        auto *abbcmp = e->GetComponent<ColliderComponent>();
        camcmp->target.x = transcmp->position.x + (abbcmp->size.x / 2);
        camcmp->target.y = transcmp->position.y + (abbcmp->size.y / 2);
      }
      else if (e->HasComponent<ShapeComponent>())
      {
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

    float distance = Vector2Distance(camcmp->target, rd->camera.target);
    float speedFactor = distance / camcmp->maxFollowDistance;

    speedFactor = std::clamp<float>(speedFactor, 0, 1);

    Vector2 velocity = Vector2Subtract(camcmp->target, rd->camera.target);
    rd->camera.target = Vector2Add(                                                                       ///
      rd->camera.target, Vector2Scale(Vector2Normalize(velocity), camcmp->cameraSpeed * speedFactor * dt) //
    );
  }
} // namespace Base
