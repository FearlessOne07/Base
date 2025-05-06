#include "base/camera/CameraManager.hpp"
#include "raylib.h"

namespace Base
{
  void CameraManager::Update(float dt)
  {
  }

  void CameraManager::BeginCameraMode()
  {
    BeginMode2D(_camera.camera);
  }

  void CameraManager::EndCameraMode()
  {
    EndMode2D();
  }
} // namespace Base
