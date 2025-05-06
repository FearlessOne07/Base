#pragma once

#include "raylib.h"
#include <cstdint>
namespace Base
{
  class CameraManager
  {
  public:
    enum struct CameraMode : uint8_t
    {
      STATIC = 0,
      BASIC_FOLLOW,
      SMOOTH_FOLLOW
    };

  public:
    void Update(float dt);
    void BeginCameraMode();
    void EndCameraMode();

    void SetCameraMode(CameraManager::CameraMode mode);
    void SetCameraOffset(Vector2 offset);
    void SetCameraTarget(Vector2 offset);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);

  private:
    class CameraManagerImpl;
    CameraManagerImpl *_impl;
  };
} // namespace Base
