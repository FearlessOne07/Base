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

  void CameraManager::SetCameraMode(Camera2DExt::CameraMode mode)
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

  // Imple
  void CameraManager::CameraManagerImpl::Update(float dt)
  {
    switch (_camera.cameraMode)
    {
    case Camera2DExt::CameraMode::BASIC_FOLLOW:
      BasicFollow(dt);
      break;
    case Camera2DExt::CameraMode::SMOOTH_FOLLOW:
      SmoothFollow(dt);
      break;
    case Camera2DExt::CameraMode::STATIC:
      _camera.camera.target = _camera.target;
      break;
    };
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
  }

  void CameraManager::CameraManagerImpl::SetCameraMode(Camera2DExt::CameraMode mode)
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
} // namespace Base
