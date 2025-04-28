#pragma once
#include <cstdint>
#include <raylib.h>
namespace Base
{
  class ParticleEmitter
  {

  public:
    ParticleEmitter();

    enum struct ParticleShape : uint8_t
    {
      RECT = 0,
      CIRCLE,
      PLYGON,
      TEXTURE
    };

    enum struct EmissionType : uint8_t
    {
      POINT = 0,
      LINE,
      AREA
    };

    EmissionType emissionType = ParticleEmitter::EmissionType::POINT;
    bool isEmitting = true;
    Vector2 position = {0, 0};

    // Particle Spec
    ParticleShape particleShape = ParticleEmitter::ParticleShape::RECT;
    Vector2 particleDirection = {0, 0};
    float particleSpeed = 0.f;
    float particleLifeTime = 0.f;

    Color particleStartColor = WHITE;
    Color particleEndColor = BLACK;

    // Rect
    Vector2 particleStartSize = {10, 10};
    Vector2 particleEndSize = {0, 0};
    Vector2 particleSize = {0, 0};

    // Circle / Polygon
    float particleStartRadius = 10.f;
    float particleEndRadius = 0.f;

    float rotationSpeed = 0.f;
  };
} // namespace Base
