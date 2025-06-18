#pragma once
#include "base/input/InputEvent.hpp"
#include <map>
#include <memory>
#include <typeindex>

namespace Base
{
  class EntityManager;
  class System;
  class Scene;
  class SystemManager
  {
  private: // References
    friend class Game;
    std::map<std::type_index, std::shared_ptr<System>> _systems;
    EntityManager *_entityManager = nullptr;
    std::type_index _renderSystemID = typeid(nullptr);

    const Scene *_currentScene = nullptr;
    bool _isSuspended = false;

  private: // Methods
    SystemManager(EntityManager *entityManager);
    void RegisterSystem(std::type_index, std::shared_ptr<System> system, bool isRenderSystem);
    void Update(float dt);
    void Init();
    void UpdateCurrentScene(const Scene *scene);

  public:
    void Suspend();
    void Unsuspend();
    void StartSystems();
    void StopSystems();
    void Render();
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
  };
} // namespace Base
