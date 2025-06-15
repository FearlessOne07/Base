#pragma once
namespace Base
{
  struct CameraShakeConfig
  {
    float trauma = 0.f;
    float traumaMultiplyer = 0.f;
    bool additiveTrauma = false;
    float frequency = 0.f;
    float shakeMagnitude = 0.f;
    float rotationMagnitude = 0.f;
    float duration = 0.f;
  };
} // namespace Base
