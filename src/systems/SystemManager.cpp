#include "base/systems/SystemManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/System.hpp"
#include "internal/scene/SceneManager.hpp"
#include "internal/systems/AreaSystem.hpp"
#include "internal/systems/EntityCollisionSystem.hpp"
#include "internal/systems/InputSystem.hpp"
#include "internal/systems/MoveSystem.hpp"
#include "internal/systems/ProximitySystem.hpp"
#include "internal/systems/RenderSystem.hpp"
#include "internal/systems/StateSystem.hpp"
#include "internal/systems/TimerSystem.hpp"
#include <memory>
#include <typeindex>
#include <utility>

namespace Base
{
  SystemManager::SystemManager(Ref<EntityManager> entityManager) : _entityManager(entityManager)
  {
  }

  void SystemManager::SetSceneManager(Ref<SceneManager> sceneManager)
  {
    _sceneManager = sceneManager;
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

  void SystemManager::Suspend()
  {
    if (!_isSuspended)
    {
      _isSuspended = true;
    }
  }

  void SystemManager::Unsuspend()
  {
    if (_isSuspended)
    {
      _isSuspended = false;
    }
  }

  void SystemManager::Init()
  {

    // Regiser Core Systems
    std::shared_ptr<RenderSystem> _rSystem = std::make_shared<RenderSystem>();
    std::shared_ptr<MoveSystem> _mvSystem = std::make_shared<MoveSystem>();
    std::shared_ptr<InputSystem> _iSystem = std::make_shared<InputSystem>();
    std::shared_ptr<EntityCollisionSystem> _ecSystem = std::make_shared<EntityCollisionSystem>();
    std::shared_ptr<ProximitySystem> _proxSystem = std::make_shared<ProximitySystem>();
    std::shared_ptr<AreaSystem> _areaSystem = std::make_shared<AreaSystem>();
    std::shared_ptr<TimerSystem> _timerSystem = std::make_shared<TimerSystem>();
    std::shared_ptr<StateSystem> _stateSystem = std::make_shared<StateSystem>();
    RegisterSystem(std::type_index(typeid(RenderSystem)), _rSystem, true);
    RegisterSystem(std::type_index(typeid(StateSystem)), _stateSystem, false);
    RegisterSystem(std::type_index(typeid(MoveSystem)), _mvSystem, false);
    RegisterSystem(std::type_index(typeid(InputSystem)), _iSystem, false);
    RegisterSystem(std::type_index(typeid(ProximitySystem)), _proxSystem, false);
    RegisterSystem(std::type_index(typeid(AreaSystem)), _areaSystem, false);
    RegisterSystem(std::type_index(typeid(TimerSystem)), _timerSystem, false);
    RegisterSystem(std::type_index(typeid(EntityCollisionSystem)), _ecSystem, false);
  }

  void SystemManager::Render()
  {
    _systems.at(_renderSystemID)->Update(0, _entityManager, _sceneManager->GetCurrentScene());
  }

  void SystemManager::Update(float dt)
  {
    if (!_isSuspended)
    {
      for (auto &[id, system] : _systems)
      {
        system->Update(dt, _entityManager, _sceneManager->GetCurrentScene());
      }
    }
  }

  void SystemManager::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    if (!_isSuspended)
    {
      for (auto &[id, system] : _systems)
      {
        system->OnInputEvent(event);
        if (event->isHandled)
        {
          break;
        }
      }
    }
  }

  void SystemManager::StartSystems()
  {
    for (auto &[id, system] : _systems)
    {
      system->Start();
    }
    _isSuspended = false;
  }

  void SystemManager::StopSystems()
  {
    for (auto &[id, system] : _systems)
    {
      system->Stop();
    }
    _isSuspended = true;
  }
} // namespace Base
