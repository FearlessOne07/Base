#pragma once

#include "base/particles/ParticleEmitter.hpp"
#include "raylib.h"
namespace Base
{
  class Particle
  {
  public:
    bool isActive = false;
    Vector2 position = {0, 0};
    Vector2 direction = {0, 0};
    float speed = 0.f;
    float lifeTime = 1.f;
    float lifeTimer = 0.f;

    ParticleEmitter::ParticleShape shape;

    Color startColor = WHITE;
    Color endColor = BLACK;

    // Rect
    Vector2 startSize = {10, 10};
    Vector2 endSize = {0, 0};

    // Circle / Polygon
    float startRadius = 10.f;
    float endRadius = 0.f;

    int sideNumber = 3;

    // Rotation
    float rotationSpeed = 0.f;
    float rotation = 0.f;
  };
} // namespace Base
