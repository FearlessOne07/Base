#pragma once
#include <map>
#include <memory>
#include <typeindex>

namespace Base
{
  class EntityManager;
  class System;
  class SystemManager
  {
  private: // References
    friend class Game;
    EntityManager *_entityManager = nullptr;
    std::map<std::type_index, std::shared_ptr<System>> _systems;
    std::type_index _renderSystemID = typeid(nullptr);

  private: // Methods
    SystemManager(EntityManager *entityManager);
    void _activateSystem(std::type_index);

  public:
    template <typename T> void ActivatSystem()
    {
      _activateSystem(typeid(T));
    }

    void DeactivateActiveSystems();
    void RegisterSystem(std::type_index, std::shared_ptr<System> system, bool isRenderSystem = false);
    void Update(float dt);
    void Render();
  };
} // namespace Base
