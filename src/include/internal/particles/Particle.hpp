#pragma once
#include "base/particles/ParticleEmitter.hpp"
#include "base/sprites/Sprite.hpp"
#include "base/util/Pauseable.hpp"
#include "raylib.h"
namespace Base
{
  class Particle : public Pauseable
  {
  public:
    bool isActive = false;
    Vector2 position = {0, 0};
    Vector2 direction = {0, 0};
    float lifeTime = 1.f;
    float lifeTimer = 0.f;

    float startSpeed = 100.f;
    float endSpeed = 0.f;

    ParticleEmitter::ParticleShape shape;

    Color startColor = WHITE;
    Color endColor = BLACK;

    // Rect
    Vector2 startSize = {10, 10};
    Vector2 endSize = {0, 0};

    // Circle / Polygon
    float startRadius = 10.f;
    float endRadius = 0.f;

    // Polygon
    int sideNumber = 3;

    // Texture
    Sprite sprite;

    // Rotation
    float rotationSpeed = 0.f;
    float rotation = 0.f;
  };
} // namespace Base
