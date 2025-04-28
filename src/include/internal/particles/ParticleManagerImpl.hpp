#pragma once
#include "base/particles/ParticleEmitter.hpp"
#include "base/particles/ParticleManager.hpp"
#include "internal/particles/Particle.hpp"
#include <array>
#include <queue>
#include <vector>

#ifndef MAX_PARTICLES
#define MAX_PARTICLES 10000
#endif // !MAX_PARTICLES

namespace Base
{
  class ParticleManager::ParticleManagerImpl
  {
  private:
    std::array<Particle, MAX_PARTICLES> _particles;
    std::vector<Particle *> _activeParticles;
    std::queue<int> _availableParticles;
    std::vector<ParticleEmitter> _emitters;

  public:
    void Init();
    void Update(float dt);
    void Render();
    ParticleEmitter *AddEmitter();
  };
} // namespace Base
