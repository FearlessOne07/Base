#pragma once
#include "base/particles/ParticleEmitter.hpp"
namespace Base
{
  class ParticleManager
  {
  private:
    friend class Game;
    ParticleManager();
    void Init();
    void Update(float dt);

  public:
    ~ParticleManager();
    void Render();
    ParticleEmitter *AddEmitter();

  private:
    class ParticleManagerImpl;
    ParticleManagerImpl *_impl;
  };
} // namespace Base
