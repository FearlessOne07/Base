#pragma once
#include "base/Exports.hpp"
#include <map>
#include <memory>
#include <typeindex>

namespace Base
{
  class EntityManager;
  class System;
  class BASEAPI SystemManager
  {
  private: // References
    friend class Game;
    EntityManager *_entityManager = nullptr;
    std::map<std::type_index, std::unique_ptr<System>> _systems;
    std::type_index _renderSystemID = typeid(nullptr);

  private: // Methods
    SystemManager(EntityManager *entityManager);
    void __activateSystem(std::type_index);

  public:
    template <typename T> void ActivatSystem()
    {
      __activateSystem(typeid(T));
    }

    void DeactivateActiveSystems();
    void RegisterSystem(std::type_index, std::unique_ptr<System> system, bool isRenderSystem = false);
    void Update(float dt);
    void Render();
  };
} // namespace Base
