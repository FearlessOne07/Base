#pragma once
#include "base/camera/CameraManager.hpp"
#include "internal/camera/Camera2DExt.hpp"

namespace Base
{
  class CameraManager::CameraManagerImpl
  {
  private:
    Camera2DExt _camera = {};

  public:
    void Update(float dt);
    void BeginCameraMode();
    void EndCameraMode();

    void SetCameraOffset(Vector2 offset);
    void SetCameraMode(CameraManager::CameraMode mode);
    void SetCameraTarget(Vector2 offset);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);
  };
} // namespace Base
