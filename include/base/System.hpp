#pragma once
#include "base/Entity.hpp"

namespace Base
{
  class EntityManager;
  class BASEAPI System
  {
  private:
    bool _isActive = false;

  public: // Virtual
    virtual ~System();
    virtual void Update(float dt, EntityManager *entityManager) = 0;

  public: // Implemented
    void Activate();
    void Deactivate();
    bool IsActive() const;
  };
} // namespace Base
