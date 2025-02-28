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

  void SystemManager::RegisterSystem(std::type_index systemID, std::unique_ptr<System> system, bool isRenderSystem)
  {
    if (_systems.find(systemID) == _systems.end())
    {
      _systems[systemID] = std::move(system);
      if (isRenderSystem)
      {
        _renderSystemID = systemID;
      }
    }
    else
    {
      throw std::runtime_error("Registration of duplicate system");
    }
  }
  void SystemManager::__activateSystem(std::type_index systemID)
  {
    if (_systems.find(systemID) != _systems.end() && !_systems.at(systemID)->IsActive())
    {
      _systems.at(systemID)->Activate();
    }
    else
    {
      throw std::runtime_error("Specified system does not exsist");
    }
  }
  void SystemManager::DeactivateActiveSystems()
  {
    for (auto &[id, system] : _systems)
    {
      if (system->IsActive())
      {
        system->Deactivate();
      }
    }
  }
  void SystemManager::Update(float dt)
  {
    for (auto &[id, system] : _systems)
    {
      if (id != _renderSystemID && system->IsActive())
      {
        system->Update(dt, _entityManager);
      }
    }
  }

  void SystemManager::Render()
  {
    if (_systems.at(_renderSystemID)->IsActive())
    {
      _systems.at(_renderSystemID)->Update(0, _entityManager);
    }
  }
} // namespace Base
