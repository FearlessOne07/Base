#pragma once
#include "base/camera/CameraManager.hpp"
#include "internal/camera/Camera2DExt.hpp"

namespace Base
{
  class CameraManager::CameraManagerImpl
  {
  private:
    Camera2DExt _camera = {};

    // Shake Variables
    Vector2 _shakeOffset = {0, 0};
    float _shakeIntensity = 0;
    float _shakeDuration = 0;
    float _shakeTimer = 0;
    Vector2 _preShakeOffset = {0, 0};

  private:
    void BasicFollow(float dt);
    void SmoothFollow(float dt);

  public:
    void Shake(float duration, float intensity);
    void Update(float dt);
    void BeginCameraMode();
    void EndCameraMode();

    void SetCameraOffset(Vector2 offset);
    void SetCameraMode(Camera2DExtMode mode);
    void SetCameraTarget(Vector2 offset);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);

    Vector2 GetScreenToWorld(Vector2 coordinate);
    Vector2 GetWorldToScreen(Vector2 coordinate);
  };
} // namespace Base
