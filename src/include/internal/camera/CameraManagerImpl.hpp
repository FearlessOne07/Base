#pragma once
#include "base/camera/Camera2DExt.hpp"
#include "base/camera/CameraManager.hpp"
#include <memory>
#include <unordered_map>

namespace Base
{
  class RenderLayer;
  class CameraManager::CameraManagerImpl
  {
  private:
    std::unordered_map<const RenderLayer *, std::shared_ptr<Camera2DExt>> _cameras;

  private:
    void BasicFollow(std::shared_ptr<Camera2DExt>, float dt);
    void SmoothFollow(std::shared_ptr<Camera2DExt>, float dt);
    void UpdateShake(float dt);
    void AddCamera(const RenderLayer *renderLayer, std::shared_ptr<Camera2DExt>);

  public:
    void Shake(const RenderLayer *renderLayer, CameraShakeConfig config);
    void Update(float dt);
    void BeginCameraMode(const RenderLayer *);
    void EndCameraMode();

    Vector2 GetScreenToWorld(const RenderLayer *, Vector2 coordinate);
    Vector2 GetWorldToScreen(const RenderLayer *, Vector2 coordinate);
  };
} // namespace Base
