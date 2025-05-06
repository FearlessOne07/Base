#include "base/particles/ParticleManager.hpp"
#include "base/particles/ParticleEmitter.hpp"
#include "base/util/Exception.hpp"
#include "internal/particles/ParticleManagerImpl.hpp"
#include "raylib.h"
#include <algorithm>
#include <random>
#include <raymath.h>

namespace Base
{
  ParticleManager::ParticleManager()
  {
    _impl = new ParticleManagerImpl;
  }

  ParticleManager::~ParticleManager()
  {
    delete _impl;
  }

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
    _randomGenerator = std::mt19937_64(_randomGenerator());
    _activeParticles.reserve(MAX_PARTICLES);
  }

  void ParticleManager::ParticleManagerImpl::InitParticleFromEmitter(ParticleEmitter &emitter, Particle *particle)
  {
    // General
    particle->lifeTime = emitter.particleLifeTime;
    particle->lifeTimer = particle->lifeTime;

    // Movement
    particle->isActive = true;
    particle->direction = emitter.particleDirection;
    particle->startSpeed = emitter.particleStartSpeed;
    particle->endSpeed = emitter.particleEndSpeed;
    particle->isActive = true;

    // Looks
    particle->startColor = emitter.particleStartColor;
    particle->endColor = emitter.particleEndColor;
    particle->shape = emitter.particleShape;
    if (                                                           //
      particle->shape == ParticleEmitter::ParticleShape::CIRCLE || //
      particle->shape == ParticleEmitter::ParticleShape::POLYGON   //
    )
    {
      particle->startRadius = emitter.particleStartRadius;
      particle->endRadius = emitter.particleEndRadius;

      if (particle->shape == ParticleEmitter::ParticleShape::POLYGON)
      {
        particle->sideNumber = emitter.particleSideNumber;
      }
    }
    else if (particle->shape == ParticleEmitter::ParticleShape::RECT)
    {
      particle->startSize = emitter.particleStartSize;
      particle->endSize = emitter.particleEndSize;
    }
    // Position

    switch (emitter.emissionType)
    {
    case ParticleEmitter::EmissionType::POINT: {
      particle->position = emitter.emissionPoint;
      break;
    }
    case ParticleEmitter::EmissionType::LINE: {
    }
      particle->position.x =
        std::uniform_real_distribution<float>(emitter.emissionLineStart.x, emitter.emissionLineEnd.x)(_randomGenerator);
      particle->position.y =
        std::uniform_real_distribution<float>(emitter.emissionLineStart.y, emitter.emissionLineEnd.y)(_randomGenerator);
      break;
    case ParticleEmitter::EmissionType::AREA: {
    }
      Vector2 minPosition = {
        emitter.emissionAreaPosition.x,
        emitter.emissionAreaPosition.y,
      };
      Vector2 maxPosition = {
        emitter.emissionAreaPosition.x + emitter.emissionAreaSize.x,
        emitter.emissionAreaPosition.y + emitter.emissionAreaSize.y,
      };

      particle->position.x = std::uniform_real_distribution<float>(minPosition.x, maxPosition.x)(_randomGenerator);
      particle->position.y = std::uniform_real_distribution<float>(minPosition.y, maxPosition.y)(_randomGenerator);
      break;
    }

    particle->rotationSpeed = emitter.particleRotationSpeed;
    _activeParticles.emplace_back(particle);
  }
  void ParticleManager::ParticleManagerImpl::Update(float dt)
  {
    // Emitters
    for (auto &emitter : _emitters)
    {
      if (emitter.isEmitting)
      {
        if (!emitter.burst)
        {
          if (                                                                            //
            emitter.emissionRate > 0 && emitter.emissionTimer >= 1 / emitter.emissionRate //
          )
          {
            emitter.emissionTimer = 0;
            Particle *particle = nullptr;

            for (auto &part : _particles)
            {
              if (!part.isActive)
              {
                particle = &part;
                break;
              }
            }

            if (!particle)
            {
              break;
            }

            InitParticleFromEmitter(emitter, particle);
          }
          else
          {
            emitter.emissionTimer += dt;
          }
        }
        else
        {
          emitter.isEmitting = false;

          for (int i = 0; i < emitter.burstEmissionCount; i++)
          {

            Particle *particle = nullptr;
            for (auto &part : _particles)
            {
              if (!part.isActive)
              {
                particle = &part;
                break;
              }
            }

            if (!particle)
            {
              break;
            }

            if (!emitter.initialisationFunction)
            {
              THROW_BASE_RUNTIME_ERROR("Initialization function not provided for burst emitter!");
            }
            emitter.initialisationFunction(emitter);
            InitParticleFromEmitter(emitter, particle);
          }
        }
      }
    }

    // Particles
    for (auto particle : _activeParticles)
    {
      float lifePoint = 1.f - (particle->lifeTimer / particle->lifeTime);
      // Position
      particle->position += particle->direction * Lerp(particle->startSpeed, particle->endSpeed, lifePoint) * dt;
      // Rotation
      particle->rotation += particle->rotationSpeed * dt;

      particle->lifeTimer -= dt;

      if (particle->lifeTimer <= 0)
      {
        particle->isActive = false;
        continue;
      }
    }
    auto dead = std::ranges::remove_if(_activeParticles, [](Particle *p) { return !p->isActive; });
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
      if (                                                            //
        particle->shape == ParticleEmitter::ParticleShape::POLYGON || //
        particle->shape == ParticleEmitter::ParticleShape::CIRCLE     //
      )
      {
        float radius = Lerp(particle->startRadius, particle->endRadius, lifePoint);
        if (particle->shape == ParticleEmitter::ParticleShape::POLYGON)
        {
          DrawPoly(particle->position, particle->sideNumber, radius, particle->rotation, color);
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
