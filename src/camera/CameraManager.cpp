#include "base/camera/CameraManager.hpp"
#include "internal/camera/Camera2DExt.hpp"
#include "internal/camera/CameraManagerImpl.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

namespace Base
{
  CameraManager::CameraManager()
  {
    _impl = new CameraManagerImpl;
  }

  CameraManager::~CameraManager()
  {
    delete _impl;
  }

  void CameraManager::Update(float dt)
  {
    _impl->Update(dt);
  }
  void CameraManager::BeginCameraMode()
  {
    _impl->BeginCameraMode();
  }
  void CameraManager::EndCameraMode()
  {
    _impl->EndCameraMode();
  }

  void CameraManager::SetCameraMode(Camera2DExtMode mode)
  {
    _impl->SetCameraMode(mode);
  }
  void CameraManager::SetCameraOffset(Vector2 offset)
  {
    _impl->SetCameraOffset(offset);
  }

  void CameraManager::SetCameraTarget(Vector2 target)
  {
    _impl->SetCameraTarget(target);
  }

  void CameraManager::SetCameraRotation(float rotation)
  {
    _impl->SetCameraRotation(rotation);
  }
  void CameraManager::SetCameraZoom(float zoom)
  {
    _impl->SetCameraZoom(zoom);
  }

  Vector2 CameraManager::GetScreenToWorld(Vector2 coordinate)
  {
    return _impl->GetScreenToWorld(coordinate);
  }
  Vector2 CameraManager::GetWorldToScreen(Vector2 coordinate)
  {
    return _impl->GetWorldToScreen(coordinate);
  }

  void CameraManager::Shake(CameraShakeConfig config)
  {
    _impl->Shake(config);
  }

  // Imple
  void CameraManager::CameraManagerImpl::Update(float dt)
  {
    switch (_camera.cameraMode)
    {
    case Camera2DExtMode::BASIC_FOLLOW:
      BasicFollow(dt);
      break;
    case Camera2DExtMode::SMOOTH_FOLLOW:
      SmoothFollow(dt);
      break;
    case Camera2DExtMode::STATIC:
      _camera.camera.target = _camera.target;
      break;
    };

    // Shake
    UpdateShake(dt);
  }

  void CameraManager::CameraManagerImpl::UpdateShake(float dt)
  {
    if (!_isShaking || _trauma <= 0.0f)
    {
      // Reset camera offset to pre-shake values if no trauma
      if (_trauma <= 0.0f && _isShaking)
      {
        _camera.camera.offset = _preShakeOffset;
        _camera.camera.rotation = _preShakeRotation;
        _isShaking = false;
      }
      return;
    }

    // Update duration if it's not indefinite (0)
    if (_shakeDuration > 0)
    {
      _shakeDuration -= dt;
      if (_shakeDuration <= 0)
      {
        // Force trauma to zero when duration ends
        _trauma = 0.0f;
        _camera.camera.offset = _preShakeOffset;
        _isShaking = false;
        return;
      }
    }

    // Decay trauma over time
    float timeLeft = _shakeDuration / _initialDuration;

    float trauma = _trauma * _traumaMultiplyer * timeLeft;

    // This gives a more natural feel to the decay
    float intensity = (trauma * trauma * trauma);

    // Update time for noise animation - scaled by frequency
    _time += dt * _frequency;

    // Get noise values for x and y
    _noise.SetSeed(_seed + 1);
    float noiseX = _noise.GetNoise(_time, 0.0f, 0.0f);

    _noise.SetSeed(_seed + 2);
    float noiseY = _noise.GetNoise(0.0f, _time, 0.0f);

    _noise.SetSeed(_seed + 3);
    float noiseRot = _noise.GetNoise(0.0f, 0.0f, _time);

    // Apply shake to camera with intensity factored in
    float offsetX = noiseX * _shakeMagnitude * intensity;
    float offsetY = noiseY * _shakeMagnitude * intensity;
    float rotation = noiseRot * _rotationMagnitude * intensity;

    _camera.camera.offset.x = offsetX + _preShakeOffset.x;
    _camera.camera.offset.y = offsetY + _preShakeOffset.y;
    _camera.camera.rotation = rotation + _preShakeRotation;
  }

  void CameraManager::CameraManagerImpl::BeginCameraMode()
  {
    BeginMode2D(_camera.camera);
  }

  void CameraManager::CameraManagerImpl::EndCameraMode()
  {
    EndMode2D();
  }

  void CameraManager::CameraManagerImpl::SetCameraOffset(Vector2 offset)
  {
    _camera.camera.offset = offset;
    _preShakeOffset = offset;
  }

  void CameraManager::CameraManagerImpl::SetCameraMode(Camera2DExtMode mode)
  {
    _camera.cameraMode = mode;
  }

  void CameraManager::CameraManagerImpl::SetCameraTarget(Vector2 target)
  {
    _camera.target = target;
  }

  void CameraManager::CameraManagerImpl::SetCameraRotation(float rotation)
  {
    _camera.camera.rotation = rotation;
    _preShakeRotation = rotation;
  }

  void CameraManager::CameraManagerImpl::SetCameraZoom(float zoom)
  {
    _camera.camera.zoom = zoom;
  }

  Vector2 CameraManager::CameraManagerImpl::GetScreenToWorld(Vector2 coordinate)
  {
    return GetScreenToWorld2D(coordinate, _camera.camera);
  }
  Vector2 CameraManager::CameraManagerImpl::GetWorldToScreen(Vector2 coordinate)
  {
    return GetWorldToScreen2D(coordinate, _camera.camera);
  }

  void CameraManager::CameraManagerImpl::BasicFollow(float dt)
  {
    _camera.camera.target = _camera.target;
  }

  void CameraManager::CameraManagerImpl::SmoothFollow(float dt)
  {
    float distance = Vector2Distance(_camera.target, _camera.camera.target);
    float speedFactor = distance / _camera.maxFollowDistance;

    speedFactor = std::clamp<float>(speedFactor, 0, 1);

    Vector2 velocity = Vector2Subtract(_camera.target, _camera.camera.target);
    _camera.camera.target = Vector2Add(                                                                       ///
      _camera.camera.target, Vector2Scale(Vector2Normalize(velocity), _camera.cameraSpeed * speedFactor * dt) //
    );
  }

  void CameraManager::CameraManagerImpl::Shake(CameraShakeConfig config)
  {
    if (!_isShaking)
    {
      _isShaking = true;
      _trauma = std::min(_trauma + config.trauma, 1.0f);
      _traumaMultiplyer = config.traumaMultiplyer;
      _frequency = config.frequency;
      _shakeMagnitude = config.shakeMagnitude;
      _rotationMagnitude = config.rotationMagnitude;
      _isShaking = true;

      _shakeDuration = config.duration;
      _initialDuration = _shakeDuration;

      // Setup noise
      _noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
      _noise.SetFrequency(_frequency / 100.0f); // Scale the frequency appropriately
      _seed = GetRandomValue(-1507525927, 1507525927);
    }
  }
} // namespace Base
