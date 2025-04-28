#pragma once

#include "base/particles/ParticleEmitter.hpp"
namespace Base
{
  class ParticleManager
  {
  public:
    void Init();
    void Update(float dt);
    void Render();
    ParticleEmitter *AddEmitter();

  private:
    class ParticleManagerImpl;
    ParticleManagerImpl *_impl;
  };
} // namespace Base
