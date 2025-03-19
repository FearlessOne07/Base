#pragma once
#include "base/Entity.hpp"

namespace Base
{
  class EntityManager;
  class   System
  {
  private:
    bool _isActive = false;

    // For the system to register events
    virtual void Start() = 0;
    // For the system to unregister events
    virtual void Stop() = 0;

  public: // Virtual
    virtual ~System() = default;

    // Update Function
    virtual void Update(float dt, EntityManager *entityManager) = 0;

  public: // Implemented
    void Activate();
    void Deactivate();
    bool IsActive() const;
  };
} // namespace Base
