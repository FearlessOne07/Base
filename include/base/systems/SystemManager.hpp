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

  private: // Methods
    SystemManager(EntityManager *entityManager);
    void _activateSystem(std::type_index);
    void RegisterSystem(std::type_index, std::shared_ptr<System> system, bool isRenderSystem);
    void Update(float dt);
    void Init();
    void UpdateCurrentScene(const Scene *scene);

  public:
    template <typename T> void ActivatSystem()
    {
      _activateSystem(typeid(T));
    }

    void DeactivateActiveSystems();
    void SuspendAllSystems();
    void UnsuspendSuspendedSystems();
    void Render();
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
  };
} // namespace Base
