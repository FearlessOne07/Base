#pragma once
#include "base/particles/ParticleEmitter.hpp"
#include "base/particles/ParticleManager.hpp"
#include "internal/particles/Particle.hpp"
#include <array>
#include <random>
#include <vector>

#ifndef MAX_PARTICLES
#define MAX_PARTICLES 10000
#endif // !MAX_PARTICLES

namespace Base
{
class Scene;
  class ParticleManager::ParticleManagerImpl
  {
  private:
    std::array<Particle, MAX_PARTICLES> _particles;
    std::vector<Particle *> _activeParticles;
    std::vector<ParticleEmitter> _emitters;
    std::random_device _randomDevice;
    std::mt19937_64 _randomGenerator;

    const Scene* _currentScene;

  private:
    void InitParticleFromEmitter(ParticleEmitter &emitter, Particle *particle);

  public:
    void Init();
    void Update(float dt);
    void Render();
    ParticleEmitter *AddEmitter();
    void UpdateCurrentScene(const Scene* scene);
  };
} // namespace Base
