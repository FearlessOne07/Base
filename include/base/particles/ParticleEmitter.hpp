#pragma once
#include "base/sprites/Sprite.hpp"
#include "base/util/Pauseable.hpp"
#include <cstdint>
#include <functional>
#include <raylib.h>
namespace Base
{
  class ParticleEmitter : public Pauseable
  {
  public:
    enum struct ParticleShape : uint8_t
    {
      RECT = 0,
      CIRCLE,
      POLYGON,
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
    float emissionRate = 1.f;
    float emissionTimer = 0.f;

    bool burst = false;
    int burstEmissionCount = 100;

    std::function<void(ParticleEmitter &emitter)> initialisationFunction = nullptr;

    // Particle Spec
    ParticleShape particleShape = ParticleEmitter::ParticleShape::RECT;
    Vector2 particleDirection = {0, 0};
    float particleLifeTime = 1.f;

    float particleStartSpeed = 100.f;
    float particleEndSpeed = 0.f;

    Color particleStartColor = WHITE;
    Color particleEndColor = BLACK;

    // Rect
    Vector2 particleStartSize = {10, 10};
    Vector2 particleEndSize = {0, 0};

    // Circle / Polygon
    float particleStartRadius = 10.f;
    float particleEndRadius = 0.f;
    int particleSideNumber = 3;

    // Texture
    Sprite particleSprite;

    Rectangle particleTextureSource = {0, 0, 0, 0};

    float particleRotationSpeed = 0.f;

    // Point
    Vector2 emissionPoint = {0, 0};

    // Area
    Vector2 emissionAreaPosition = {0, 0};
    Vector2 emissionAreaSize = {100, 100};

    // Line
    Vector2 emissionLineStart = {0, 0};
    Vector2 emissionLineEnd = {0, 0};
  };
} // namespace Base
