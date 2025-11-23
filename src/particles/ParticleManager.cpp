#include "base/particles/ParticleManager.hpp"
#include "base/particles/ParticleEmitter.hpp"
#include "base/scenes/Scene.hpp"
#include "base/util/Draw.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Ref.hpp"
#include "internal/particles/ParticleManagerImpl.hpp"
#include "internal/scene/SceneManager.hpp"
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

  void ParticleManager::Init(Ref<SceneManager> sceneManager)
  {
    _impl->Init(sceneManager);
  }

  void ParticleManager::Update(float dt)
  {
    _impl->Update(dt);
  }

  void ParticleManager::Render()
  {
    _impl->Render();
  }

  Ref<ParticleEmitter> ParticleManager::AddEmitter()
  {
    return _impl->AddEmitter();
  }

  void ParticleManager::ParticleManagerImpl::Init(Ref<SceneManager> sceneManager)
  {
    _sceneManager = sceneManager;
    _randomGenerator = std::mt19937_64(_randomGenerator());
    _activeParticles.reserve(MAX_PARTICLES);
  }

  void ParticleManager::ParticleManagerImpl::InitParticleFromEmitter(ParticleEmitter &emitter, Particle *particle)
  {
    // General
    particle->SetPauseMask(emitter.GetPauseMask());
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
      particle->shape == ParticleEmitter::ParticleShape::Circle || //
      particle->shape == ParticleEmitter::ParticleShape::Polygon   //
    )
    {
      particle->startRadius = emitter.particleStartRadius;
      particle->endRadius = emitter.particleEndRadius;

      if (particle->shape == ParticleEmitter::ParticleShape::Polygon)
      {
        particle->sideNumber = emitter.particleSideNumber;
      }
    }
    else if ( //
      particle->shape == ParticleEmitter::ParticleShape::Rect ||
      particle->shape == ParticleEmitter::ParticleShape::Texture //
    )
    {
      particle->startSize = emitter.particleStartSize;
      particle->endSize = emitter.particleEndSize;

      if (particle->shape == ParticleEmitter::ParticleShape::Texture)
      {
        if (!emitter.particleSprite)
        {
          THROW_BASE_RUNTIME_ERROR("Sprite not provided for textured particle emitter");
        }
        particle->sprite = emitter.particleSprite;
      }
    }

    // Position
    switch (emitter.emissionType)
    {
    case ParticleEmitter::EmissionType::Point: {
      particle->position = emitter.emissionPoint;
      break;
    }
    case ParticleEmitter::EmissionType::Line: {
    }
      particle->position.x =
        std::uniform_real_distribution<float>(emitter.emissionLineStart.x, emitter.emissionLineEnd.x)(_randomGenerator);
      particle->position.y =
        std::uniform_real_distribution<float>(emitter.emissionLineStart.y, emitter.emissionLineEnd.y)(_randomGenerator);
      break;
    case ParticleEmitter::EmissionType::Area: {
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
      auto A = _sceneManager->GetCurrentScene()->GetPauseMask();
      auto B = emitter.GetPauseMask();
      if (emitter.GetPauseMask().count() != 0 && (A & B) == B)
      {
        continue;
      }

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
      auto A = _sceneManager->GetCurrentScene()->GetPauseMask();
      auto B = particle->GetPauseMask();
      if (particle->GetPauseMask().count() != 0 && (A & B) == B)
      {
        continue;
      }
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
        particle->shape == ParticleEmitter::ParticleShape::Polygon || //
        particle->shape == ParticleEmitter::ParticleShape::Circle     //
      )
      {
        float radius = Lerp(particle->startRadius, particle->endRadius, lifePoint);
        if (particle->shape == ParticleEmitter::ParticleShape::Polygon)
        {
          DrawPoly(particle->position, particle->sideNumber, radius, particle->rotation, color);
        }
        else
        {
          DrawCircleV(particle->position, radius, color);
        }
      }
      else if ( //
        particle->shape == ParticleEmitter::ParticleShape::Rect ||
        particle->shape == ParticleEmitter::ParticleShape::Texture //
      )
      {
        Vector2 size = Vector2Lerp(particle->startSize, particle->endSize, lifePoint);

        if (particle->shape == ParticleEmitter::ParticleShape::Rect)
        {
          DrawRectangleBase( //
            {
              particle->position.x,
              particle->position.y,
              size.x,
              size.y,
            },
            {size.x / 2, size.y / 2}, particle->rotation, color //
          );
        }
        else if (particle->shape == ParticleEmitter::ParticleShape::Texture)
        {
          particle->sprite.SetTargetSize(size);
          particle->sprite.Render(particle->position, particle->rotation, color);
        }
      }
    }
  }

  Ref<ParticleEmitter> ParticleManager::ParticleManagerImpl::AddEmitter()
  {
    _emitters.emplace_back();
    return _emitters.back();
  }
} // namespace Base
