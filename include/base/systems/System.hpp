#pragma once
#include "base/entities/Entity.hpp"
#include "base/input/InputEvent.hpp"

namespace Base
{
  class EntityManager;
  class System
  {
  private:
    bool _isActive = false;

    // For the system to register events
    virtual void Start();
    // For the system to unregister events
    virtual void Stop();

  public: // Virtual
    virtual ~System() = default;

    // Update Function
    virtual void Update(float dt, EntityManager *entityManager) = 0;

  public: // Implemented
    void Activate();
    void Deactivate();
    bool IsActive() const;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> event);
  };
} // namespace Base
