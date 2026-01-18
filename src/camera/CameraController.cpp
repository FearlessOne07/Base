#include "base/camera/CameraController.hpp"
#include "base/camera/CameraModes.hpp"
#include "base/util/Type.hpp"
#include "glm/geometric.hpp"
#include "internal/rendering/Renderer.hpp"
#include <algorithm>
#include <memory>
#include <random>

namespace Base
{
  CameraController::CameraController(Vector2 viewPort)
  {
    _camera = std::make_shared<Camera>(viewPort);
  }

  void CameraController::Shake(const CameraShakeConfig &config)
  {
    // Mark camera as shaking
    _isShaking = true;

    // If trouma is additive
    if (config.additiveTrauma)
    {
      // Add and clamp to 1.0
      _trauma = std::min(_trauma + config.trauma, 1.0f);
    }
    else
    {
      // Else, set trouma to new value
      _trauma = config.trauma;
    }

    _traumaMultiplyer = config.traumaMultiplyer;
    _frequency = config.frequency;
    _shakeMagnitude = config.shakeMagnitude;
    _rotationMagnitude = config.rotationMagnitude;
    _isShaking = true;

    _shakeDuration = config.duration;
    _initialDuration = _shakeDuration;

    // Setup noise
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int64_t> dist(-1507525927, 1507525927);
    _noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    _noise.SetFrequency(_frequency / 100.0f); // Scale the frequency appropriately
    _seed = dist(gen);
  }

  void CameraController::UpdateShake(float dt)
  {
    // If camera isn't shaking or truam is 0
    if (!_isShaking || _trauma <= 0.0f)
    {
      // Reset camera offset to pre-shake values
      if (_trauma <= 0.0f && _isShaking)
      {
        // TODO: Implment Camera Offset
        // _camera->SetViewPort _preShakeOffset;
        _camera->SetRotation(_preShakeRotation);
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
        // Force trauma to zero when duration end reset offset and rotation
        _trauma = 0.0f;
        // _camera.offset = _preShakeOffset;
        _camera->SetRotation(_preShakeRotation);
        _isShaking = false;
        return;
      }
    }

    // Decay trauma over time
    float timeLeft = _shakeDuration / _initialDuration;

    // Scale truama by multiplyer and timeleft (tween)
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

    // Get noise value for roation
    _noise.SetSeed(_seed + 3);
    float noiseRot = _noise.GetNoise(0.0f, 0.0f, _time);

    // Apply shake to camera with intensity factored in
    float offsetX = noiseX * _shakeMagnitude * intensity;
    float offsetY = noiseY * _shakeMagnitude * intensity;
    float rotation = noiseRot * _rotationMagnitude * intensity;

    // Add offsets values
    Vector2 offset = {offsetX + _preShakeOffset.x, offsetY + _preShakeOffset.y};
    // _camera.Set
    _camera->SetRotation(rotation + _preShakeRotation);
  }

  void CameraController::Update(float dt)
  {
    switch (_cameraMode)
    {
    case CameraMode::BasicFollow:
      BasicFollow(dt);
      break;
    case CameraMode::SmoothFollow:
      SmoothFollow(dt);
      break;
    case CameraMode::Static:
      _camera->SetPosition(_target);
      break;
    };

    UpdateShake(dt);
  }

  void CameraController::BasicFollow(float dt)
  {
    _camera->SetPosition(_target);
  }

  void CameraController::SmoothFollow(float dt)
  {
    // Get distance to target
    float distance = glm::distance(_target, _camera->GetPosition());
    // Get speed factor and clamp to 0 - 1.0
    float speedFactor = distance / _maxFollowDistance;
    speedFactor = std::clamp<float>(speedFactor, 0, 1);

    // Get direction
    Vector2 velocity = _target - _camera->GetPosition();

    //  scale speed by distance left
    _camera->SetPosition(_camera->GetPosition() + (glm::normalize(velocity) * _cameraSpeed * speedFactor * dt));
  }

  Vector2 CameraController::GetScreenToWorld(Vector2 position) const
  {
    // return GetScreenToWorld2D(position, _camera);
  }

  Vector2 CameraController::GetWorldToScreen(Vector2 position) const
  {
    // return GetWorldToScreen2D(position, _camera) / _camera.zoom;
  }

  float CameraController::GetZoom() const
  {
    return _camera->GetZoom();
  }

  void CameraController::Begin()
  {
    Renderer::BeginCamera(_camera);
  }

  void CameraController::End()
  {
    Renderer::EndCamera();
  }

  void CameraController::SetMode(CameraMode mode)
  {
    _cameraMode = mode;
  }

  void CameraController::SetOffset(Vector2 offset)
  {
    _preShakeOffset = offset;
    // _camera.offset = offset;
  }

  void CameraController::SetTarget(Vector2 target)
  {
    _camera->SetPosition(target);
  }

  void CameraController::SetRotation(float rotation)
  {
    _preShakeRotation = rotation;
    _camera->SetRotation(rotation);
  }

  void CameraController::SetZoom(float zoom)
  {
    _camera->SetRotation(zoom);
  }
} // namespace Base
