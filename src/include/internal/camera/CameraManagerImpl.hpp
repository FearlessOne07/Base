#pragma once
#include "base/camera/CameraManager.hpp"
#include "internal/camera/Camera2DExt.hpp"
#include "internal/utils/FastNoiseLite.hpp"

namespace Base
{
  class CameraManager::CameraManagerImpl
  {
  private:
    Camera2DExt _camera = {};

    // Shake Variables
    FastNoiseLite _noise;

    float _trauma = 0.5;
    float _traumaMulitpyer = 5.f;

    float _shakeMagnitude = 10.f;

    float _time = 0.f;
    Vector2 _preShakeOffset = {0, 0};
    float _preShakeRotation = 0;

  private:
    void BasicFollow(float dt);
    void SmoothFollow(float dt);
    void UpdateShake(float dt);

  public:
    void Shake(CameraShakeConfig config);
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
