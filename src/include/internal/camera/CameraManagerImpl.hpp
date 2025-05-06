#pragma once
#include "base/camera/CameraManager.hpp"
#include "internal/camera/Camera2DExt.hpp"

namespace Base
{
  class CameraManager::CameraManagerImpl
  {
  private:
    Camera2DExt _camera = {};

  private:
    void BasicFollow(float dt);
    void SmoothFollow(float dt);

  public:
    void Update(float dt);
    void BeginCameraMode();
    void EndCameraMode();

    void SetCameraOffset(Vector2 offset);
    void SetCameraMode(Camera2DExtMode mode);
    void SetCameraTarget(Vector2 offset);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);
  };
} // namespace Base
