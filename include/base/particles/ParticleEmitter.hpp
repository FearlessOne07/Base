#pragma once
#include "base/rendering/Sprite.hpp"
#include "base/util/Easings.hpp"
#include "base/util/Pauseable.hpp"
#include <cstdint>
#include <functional>
namespace Base
{
  class ParticleEmitter : public Pauseable
  {
  public:
    enum struct ParticleShape : uint8_t
    {
      Rect = 0,
      Circle,
      Polygon,
      Texture
    };

    enum struct EmissionType : uint8_t
    {
      Point = 0,
      Line,
      Area
    };

    EmissionType emissionType = ParticleEmitter::EmissionType::Point;
    bool isEmitting = true;
    float emissionRate = 1.f;
    float emissionTimer = 0.f;
    Easings::Type easingType = Easings::Type::Linear;

    bool burst = false;
    int burstEmissionCount = 100;

    std::function<void(ParticleEmitter &emitter)> initialisationFunction = nullptr;

    // Particle Spec
    ParticleShape particleShape = ParticleEmitter::ParticleShape::Rect;
    Vector2 particleDirection = {0, 0};
    float particleLifeTime = 1.f;

    float particleStartSpeed = 100.f;
    float particleEndSpeed = 0.f;

    Color particleStartColor = {255, 255, 255, 255};
    Color particleEndColor = {0, 0, 0, 255};

    // Rect
    Vector2 particleStartSize = {10, 10};
    Vector2 particleEndSize = {0, 0};

    // Circle / Polygon
    float particleStartRadius = 10.f;
    float particleEndRadius = 0.f;
    int particleSideNumber = 3;

    // Texture
    Sprite particleSprite;

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
