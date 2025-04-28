#include "base/particles/ParticleEmitter.hpp"
#include "internal/particles/ParticleManagerImpl.hpp"
#include "raylib.h"
#include <algorithm>
#include <raymath.h>

namespace Base
{
  void ParticleManager::Init()
  {
    _impl->Init();
  }

  void ParticleManager::Update(float dt)
  {
    _impl->Update(dt);
  }

  void ParticleManager::Render()
  {
    _impl->Render();
  }

  ParticleEmitter *ParticleManager::AddEmitter()
  {
    return _impl->AddEmitter();
  }

  void ParticleManager::ParticleManagerImpl::Init()
  {
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
      _availableParticles.push(i);
    }
    _activeParticles.reserve(MAX_PARTICLES);
  }

  void ParticleManager::ParticleManagerImpl::Update(float dt)
  {

    // Emitters
    for (auto &emitter : _emitters)
    {
      if (emitter.isEmitting)
      {
        int nextParticle = _availableParticles.front();
        _availableParticles.pop();

        Particle &particle = _particles[nextParticle];
        particle.index = nextParticle;

        // General
        particle.lifeTime = emitter.particleLifeTime;
        particle.lifeTimer = particle.lifeTime;

        // Movement
        particle.isActive = true;
        particle.position = emitter.position;
        particle.direction = emitter.particleDirection;
        particle.speed = emitter.particleSpeed;
        particle.isActive = true;

        // Looks
        particle.startColor = emitter.particleStartColor;
        particle.endColor = emitter.particleEndColor;
        particle.shape = emitter.particleShape;
        if (                                                          //
          particle.shape == ParticleEmitter::ParticleShape::CIRCLE || //
          particle.shape == ParticleEmitter::ParticleShape::PLYGON    //
        )
        {
          particle.startRadius = emitter.particleStartRadius;
          particle.endRadius = emitter.particleEndRadius;
        }
        else if (particle.shape == ParticleEmitter::ParticleShape::RECT)
        {
          particle.startSize = emitter.particleStartSize;
          particle.endSize = emitter.particleEndSize;
        }

        particle.rotationSpeed = emitter.rotationSpeed;
        _activeParticles.emplace_back(&particle);
      }
    }

    // Particles
    for (auto &particle : _activeParticles)
    {
      // Position
      particle->position += particle->direction * particle->speed * dt;
      // Rotation
      particle->rotation += particle->rotationSpeed;

      particle->lifeTimer -= dt;

      if (particle->lifeTimer <= 0)
      {
        particle->isActive = false;
        _availableParticles.push(particle->index);
      }
    }

    auto dead = std::ranges::remove_if(_activeParticles, [](Particle *part) { return !part->isActive; });
    _activeParticles.erase(dead.begin(), dead.end());
  }

  void ParticleManager::ParticleManagerImpl::Render()
  {
    for (auto &particle : _activeParticles)
    {
      float lifePoint = 1.f - (particle->lifeTimer / particle->lifeTime);

      // Color
      Color color;
      color.r = static_cast<unsigned char>(Lerp(particle->startColor.r, particle->endColor.r, lifePoint));
      color.g = static_cast<unsigned char>(Lerp(particle->startColor.g, particle->endColor.g, lifePoint));
      color.b = static_cast<unsigned char>(Lerp(particle->startColor.b, particle->endColor.b, lifePoint));
      color.a = static_cast<unsigned char>(Lerp(particle->startColor.a, particle->endColor.a, lifePoint));

      // Size
      if (                                                           //
        particle->shape == ParticleEmitter::ParticleShape::PLYGON || //
        particle->shape == ParticleEmitter::ParticleShape::CIRCLE    //
      )
      {
        float radius = Lerp(particle->startRadius, particle->endRadius, lifePoint);

        if (particle->shape == ParticleEmitter::ParticleShape::PLYGON)
        {
          DrawPoly(particle->position, 3, radius, particle->rotation, color);
        }
        else
        {
          DrawCircleV(particle->position, radius, color);
        }
      }
      else if (particle->shape == ParticleEmitter::ParticleShape::RECT)
      {
        Vector2 size = Vector2Lerp(particle->startSize, particle->endSize, lifePoint);
        DrawRectanglePro( //
          {
            particle->position.x,
            particle->position.y,
            size.x,
            size.y,
          },
          {size.x / 2, size.y / 2}, particle->rotation, color //
        );
      }
    }
  }

  ParticleEmitter *ParticleManager::ParticleManagerImpl::AddEmitter()
  {
    _emitters.emplace_back();
    return &_emitters.back();
  }
} // namespace Base
