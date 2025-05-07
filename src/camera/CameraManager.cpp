#include "base/camera/CameraManager.hpp"
#include "internal/camera/Camera2DExt.hpp"
#include "internal/camera/CameraManagerImpl.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>

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
    // Update time for noise animation
    _time += (float)std::pow(_trauma, 2) * dt * _traumaMulitpyer;

    // Get noise values for x, y and rotation
    int seed = GetRandomValue(1, 10000); // Random seed using Raylib's random

    _noise.SetSeed(seed + 1);
    float offsetX = _noise.GetNoise(_time, 0.0f, 0.0f) * _shakeMagnitude;

    _noise.SetSeed(seed + 2);
    float offsetY = _noise.GetNoise(0.0f, _time, 0.0f) * _shakeMagnitude;

    _noise.SetSeed(seed + 3);
    float rotation = _noise.GetNoise(0.0f, 0.0f, _time) * _shakeMagnitude;

    // Apply to camera
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
    _trauma = std::min(_trauma + config.trauma, 1.0f);

    _traumaMulitpyer = config.traumaMulitpyer;
    _shakeMagnitude = config.shakeMagnitude;

    // Setup noise
    _noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  }
} // namespace Base
