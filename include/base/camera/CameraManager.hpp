#pragma once
#include "base/camera/CameraModes.hpp"
#include "raylib.h"
namespace Base
{
  class CameraManager
  {
  public:
    struct CameraShakeConfig
    {
      float trauma = 0.f;
      float traumaRecoverySpeed = 0.f;
      float maxShakeOffset = 0.f;
      float maxShakeAngle = 0.f;
      float noiseSpeed = 0.f;
      float noiseFrequency = 0.f;
      float exponent = 0.f;
    };

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
    void Shake(CameraShakeConfig config);

  private:

  private:
    class CameraManagerImpl;
    CameraManagerImpl *_impl;
  };
} // namespace Base
