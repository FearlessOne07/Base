#pragma once
#include "base/input/InputEvent.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/util/Ref.hpp"
#include <map>
#include <memory>
#include <typeindex>

namespace Base
{
  class EntityManager;
  class System;
  class SceneID;
  class SceneManager;
  class SystemManager
  {
  private: // References
    friend class Game;
    std::map<std::type_index, std::shared_ptr<System>> _systems;
    Ref<EntityManager> _entityManager;
    Ref<SceneManager> _sceneManager;
    std::type_index _renderSystemID = typeid(nullptr);

    bool _isSuspended = false;

  private: // Methods
    SystemManager(Ref<EntityManager> entityManager);
    void RegisterSystem(std::type_index, std::shared_ptr<System> system, bool isRenderSystem);
    void Update(float dt);
    void Init();
    void SetSceneManager(Ref<SceneManager> sceneManager);

  public:
    void Suspend();
    void Unsuspend();
    void StartSystems();
    void StopSystems();
    void Render();
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
  };
} // namespace Base
