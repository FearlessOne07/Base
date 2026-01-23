#pragma once
#include "base/entities/Entity.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/input/InputEvent.hpp"
#include "base/util/Ref.hpp"
#include <memory>

namespace Base
{
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
    virtual void Update(float dt, Ref<EntityManager> entityManager, std::shared_ptr<Scene> currentScene) = 0;

  public: // Implemented
    virtual void OnInputEvent(std::shared_ptr<InputEvent> event);
  };
} // namespace Base
