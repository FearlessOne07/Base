#include "base/camera/Camera2DExt.hpp"
#include "base/camera/CameraModes.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

namespace Base
{
  void Camera2DExt::Shake(const CameraShakeConfig &config)
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
    _noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    _noise.SetFrequency(_frequency / 100.0f); // Scale the frequency appropriately
    _seed = GetRandomValue(-1507525927, 1507525927);
  }

  void Camera2DExt::UpdateShake(float dt)
  {
    // If camera isn't shaking or truam is 0
    if (!_isShaking || _trauma <= 0.0f)
    {
      // Reset camera offset to pre-shake values
      if (_trauma <= 0.0f && _isShaking)
      {
        _camera.offset = _preShakeOffset;
        _camera.rotation = _preShakeRotation;
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
        _camera.offset = _preShakeOffset;
        _camera.rotation = _preShakeRotation;
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
    _camera.offset.x = offsetX + _preShakeOffset.x;
    _camera.offset.y = offsetY + _preShakeOffset.y;
    _camera.rotation = rotation + _preShakeRotation;
  }

  void Camera2DExt::Update(float dt)
  {
    switch (_cameraMode)
    {
    case Camera2DExtMode::BASIC_FOLLOW:
      BasicFollow(dt);
      break;
    case Camera2DExtMode::SMOOTH_FOLLOW:
      SmoothFollow(dt);
      break;
    case Camera2DExtMode::STATIC:
      _camera.target = _target;
      break;
    };

    UpdateShake(dt);
  }

  void Camera2DExt::BasicFollow(float dt)
  {
    _camera.target = _target;
  }

  void Camera2DExt::SmoothFollow(float dt)
  {
    // Get distance to target
    float distance = Vector2Distance(_target, _camera.target);
    // Get speed factor and clamp to 0 - 1.0
    float speedFactor = distance / _maxFollowDistance;
    speedFactor = std::clamp<float>(speedFactor, 0, 1);
    
    // Get direction
    Vector2 velocity = Vector2Subtract(_target, _camera.target);

    //  scale speed by distance left
    _camera.target = Vector2Add(                                                                ///
      _camera.target, Vector2Scale(Vector2Normalize(velocity), _cameraSpeed * speedFactor * dt) //
    );
  }

  Vector2 Camera2DExt::GetScreenToWorld(Vector2 position) const
  {
    return GetScreenToWorld2D(position, _camera);
  }

  void Camera2DExt::Begin()
  {
    BeginMode2D(_camera);
  }

  void Camera2DExt::End()
  {
    EndMode2D();
  }

  void Camera2DExt::SetMode(Camera2DExtMode mode)
  {
    _cameraMode = mode;
  }

  void Camera2DExt::SetOffset(Vector2 offset)
  {
    _preShakeOffset = offset;
    _camera.offset = offset;
  }

  void Camera2DExt::SetTarget(Vector2 target)
  {
    _camera.target = target;
  }

  void Camera2DExt::SetRotation(float rotation)
  {
    _preShakeRotation = rotation;
    _camera.rotation = rotation;
  }

  void Camera2DExt::SetZoom(float zoom)
  {
    _camera.zoom = zoom;
  }
} // namespace Base
