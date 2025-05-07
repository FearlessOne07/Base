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
    float _trauma = 0.0f;
    float _traumaMultiplyer = 2.0f;
    float _frequency = 10.0f;
    float _shakeMagnitude = 10.0f;
    float _rotationMagnitude = 0.f;
    float _time = 0.0f;
    float _shakeDuration = 0.0f;
    int _seed = 0;

    bool _isShaking = false; // Is shake active
    Vector2 _preShakeOffset = {0, 0};
    float _preShakeRotation = 0;
    float _initialTrauma = 0.f;
    float _initialDuration = 0.f;

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
