#include "base/systems/SystemManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/systems/System.hpp"
#include <memory>
#include <utility>
namespace Base
{
  SystemManager::SystemManager(EntityManager *entityManager) : _entityManager(entityManager)
  {
  }

  void SystemManager::UpdateCurrentScene(const Scene *scene)
  {
    _currentScene = scene;
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

  void SystemManager::SuspendAllSystems()
  {
    for (auto &[id, system] : _systems)
    {
      if (system->IsActive())
      {
        system->Suspend();
      }
    }
  }

  void SystemManager::UnsuspendSuspendedSystems()
  {
    for (auto &[id, system] : _systems)
    {
      if (system->IsActive() && system->IsSuspended())
      {
        system->UnSuspend();
      }
    }
  }

  void SystemManager::Init()
  {
    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> sig) {
      auto scenePushed = std::static_pointer_cast<ScenePushedSignal>(sig);
      UpdateCurrentScene(scenePushed->scene);
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> sig) {
      auto sceneResumed = std::static_pointer_cast<SceneResumedSignal>(sig);
      UpdateCurrentScene(sceneResumed->scene);
    });
  }

  void SystemManager::Update(float dt)
  {
    for (auto &[id, system] : _systems)
    {
      if (id != _renderSystemID && system->IsActive() && !system->IsSuspended())
      {
        system->Update(dt, _entityManager, _currentScene);
      }
    }
  }

  void SystemManager::Render()
  {
    if (_systems.find(_renderSystemID) != _systems.end())
    {
      if (_systems.at(_renderSystemID)->IsActive())
      {
        _systems.at(_renderSystemID)->Update(0, _entityManager, _currentScene);
      }
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("No Render System has been registered");
    }
  }

  void SystemManager::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    for (auto &[id, system] : _systems)
    {
      if (system->IsActive())
      {
        system->OnInputEvent(event);
        if (event->isHandled)
        {
          break;
        }
      }
    }
  };
} // namespace Base
