#include "base/systems/SystemManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/systems/System.hpp"
#include <memory> #include <utility>
namespace Base
{
  SystemManager::SystemManager(EntityManager *entityManager) : _entityManager(entityManager)
  {
  }

  void SystemManager::RegisterSystem(std::type_index systemID, std::shared_ptr<System> system, bool isRenderSystem)
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
      THROW_BASE_RUNTIME_ERROR("Registration of duplicate system");
    }
  }
  void SystemManager::_activateSystem(std::type_index systemID)
  {
    if (_systems.find(systemID) != _systems.end() && !_systems.at(systemID)->IsActive())
    {
      _systems.at(systemID)->Activate();
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Specified system does not exsist");
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
    if (_systems.find(_renderSystemID) != _systems.end())
    {
      if (_systems.at(_renderSystemID)->IsActive())
      {
        _systems.at(_renderSystemID)->Update(0, _entityManager);
      }
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("No Render System has been registered");
    }
  }

  void SystemManager::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
    for (auto &[id, system] : _systems)
    {
      if (system->IsActive())
      {
        system->OnInputEvent(event);
      }
    }
  };
} // namespace Base
