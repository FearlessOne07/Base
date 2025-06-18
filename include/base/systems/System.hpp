#pragma once
#include "base/entities/Entity.hpp"
#include "base/input/InputEvent.hpp"

namespace Base
{
  class EntityManager;
  class Scene;
  class System
  {
    friend class SystemManager;

  private:
    // For the system to register events
    virtual void Start();
    // For the system to unregister events
    virtual void Stop();

  public: // Virtual
    virtual ~System() = default;

    // Update Function
    virtual void Update(float dt, EntityManager *entityManager, const Scene *currentScene) = 0;

  public: // Implemented
    virtual void OnInputEvent(std::shared_ptr<InputEvent> event);
  };
} // namespace Base
