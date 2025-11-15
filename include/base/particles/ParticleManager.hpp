#pragma once
#include "base/particles/ParticleEmitter.hpp"
#include "base/util/Ref.hpp"
namespace Base
{
  class Particle;
  class SceneManager;
  class ParticleManager
  {
  private:
    friend class Game;
    ParticleManager();
    void Init(Ref<SceneManager> sceneManager);
    void Update(float dt);

  public:
    ~ParticleManager();
    void Render();
    Ref<ParticleEmitter> AddEmitter();

  private:
    class ParticleManagerImpl;
    ParticleManagerImpl *_impl;
  };
} // namespace Base
