#pragma once
#include "base/Exports.hpp"
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace Base
{
  class EntityManager;
  class System;
  class BASEAPI SystemManager
  {
  private: // References
    friend class Game;
    EntityManager *_entityManager = nullptr;
    std::unordered_map<std::type_index, std::unique_ptr<System>> _systems;

  private: // Methods
    SystemManager(EntityManager *entityManager);
    void __activateSystem(std::type_index);
    void __deactivateSystem(std::type_index);

  public:
    template <typename T> void ActivatSystem()
    {
      __activateSystem(typeid(T));
    }

    template <typename T> void DeactivatSystem()
    {
      __deactivateSystem(typeid(T));
    }
    void RegisterSystem(std::type_index, std::unique_ptr<System> system);
    void Update(float dt);
  };
} // namespace Base
