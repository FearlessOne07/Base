#include "base/SystemManager.hpp"
#include "base/EntityManager.hpp"
#include "base/System.hpp"
#include <memory>
#include <stdexcept>
#include <utility>

namespace Base
{
  SystemManager::SystemManager(EntityManager *entityManager) : _entityManager(entityManager)
  {
  }

  void SystemManager::RegisterSystem(std::type_index systemID, std::unique_ptr<System> system)
  {
    if (_systems.find(systemID) != _systems.end())
    {
      _systems[systemID] = std::move(system);
    }
    else
    {
      throw std::runtime_error("Registration of duplicate system");
    }
  }
  void SystemManager::__activateSystem(std::type_index systemID)
  {
    if (_systems.find(systemID) != _systems.end())
    {
      _systems.at(systemID)->Activate();
    }
    else
    {
      throw std::runtime_error("Specified system does not exsist");
    }
  }
  void SystemManager::__deactivateSystem(std::type_index systemID)
  {
    if (_systems.find(systemID) != _systems.end())
    {
      _systems.at(systemID)->Deactivate();
    }
    else
    {
      throw std::runtime_error("Specified system does not exsist");
    }
  }
  void SystemManager::Update(float dt)
  {
    for (auto &[id, system] : _systems)
    {
      if (system->IsActive())
      {
        system->Update(dt, _entityManager);
      }
    }
  }
} // namespace Base
