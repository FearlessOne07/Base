#pragma once
#include "base/camera/Camera.hpp"
#include "base/camera/CameraModes.hpp"
#include "base/camera/CameraShakeConfig.hpp"
#include "base/util/FastNoiseLite.hpp"
#include "base/util/Pauseable.hpp"
#include "base/util/Type.hpp"
namespace Base
{
  class CameraController : public Pauseable
  {
  private:
    // Base Raylib Camera
    Ptr<Camera> _camera;

    // Spec
    CameraMode _cameraMode;
    Vector2 _target = {0, 0};
    float _maxFollowDistance = 100.f;
    float _cameraSpeed = 0.f;

    // Shake Configs
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
    void UpdateShake(float dt);
    void BasicFollow(float dt);
    void SmoothFollow(float dt);

  public:
    void SetMode(CameraMode mode);
    void SetTarget(Vector2 target);
    void SetOriginPoint(Origin origin);
    void SetRotation(float rotation);
    void SetZoom(float zoom);
    void Shake(const CameraShakeConfig &config);
    void Update(float dt);
    Vector2 GetScreenToWorld(Vector2 position) const;
    Vector2 GetWorldToScreen(Vector2 position) const;
    float GetScreenToWorld(float distance) const;
    float GetWorldToScreen(float distance) const;
    float GetZoom() const;
    void Begin();
    void End();
    CameraController() = default;
    CameraController(Vector2 viewPort);
  };

} // namespace Base
