#pragma once
#include "base/camera/CameraModes.hpp"
#include "raylib.h"
namespace Base
{
  class CameraManager
  {
  public:
    CameraManager();
    ~CameraManager();
    void Update(float dt);
    void BeginCameraMode();
    void EndCameraMode();

    void SetCameraMode(Camera2DExtMode mode);
    void SetCameraOffset(Vector2 offset);
    void SetCameraTarget(Vector2 target);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);

    Vector2 GetScreenToWorld(Vector2 coordinate);
    Vector2 GetWorldToScreen(Vector2 coordinate);

  private:
    class CameraManagerImpl;
    CameraManagerImpl *_impl;

  private:
  };
} // namespace Base
