#include "internal/scene/SceneManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/scenes/signals/SceneSuspendedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/systems/SystemManager.hpp"
#include "base/ui/UIManager.hpp"
#include "base/util/Exception.hpp"
#include <memory>
#include <utility>

namespace Base
{
  SceneManager::SceneManager(const GameContext &ctx) : _ctx(ctx)
  {
  }

  void SceneManager::PushScene(std::type_index scene, const SceneData &sceneData)
  {
    auto bus = SignalBus::GetInstance();
    if (!_scenes.empty())
    {
      std::shared_ptr<SceneSuspendedSignal> sig = std::make_shared<SceneSuspendedSignal>();
      sig->Scene = _scenes.top()->GetSceneID();
      bus->BroadCastSignal(sig);

      // Supspend the current scene
      _scenes.top()->Suspend();
    }

    // Push new scen to the stack and enter it
    if (_factories.contains(scene))
    {
      _scenes.push(_factories.at(scene)());
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Specified Scene is not registered");
    }

    _scenes.top()->Init();
    _scenes.top()->SetSceneID((SceneID)_currentSceneID++);
    _scenes.top()->SetGameCtx(_ctx);

    std::shared_ptr<ScenePushedSignal> sig = std::make_shared<ScenePushedSignal>();
    sig->Scene = _scenes.top()->GetSceneID();
    bus->BroadCastSignal(sig);

    _scenes.top()->Enter(sceneData);
  }

  void SceneManager::PopScene()
  {
    auto bus = SignalBus::GetInstance();
    if (!_scenes.empty())
    {
      // Broadcast Scene Popped Signal
      std::shared_ptr<ScenePoppedSignal> sig = std::make_shared<ScenePoppedSignal>();
      sig->Scene = _scenes.top()->GetSceneID();
      bus->BroadCastSignal(sig);

      // Exit the current scene and pop it off the stack
      _scenes.top()->_exit();
      _scenes.pop();
    }

    // Enter the scene below it if there is one
    std::shared_ptr<SceneResumedSignal> sig = std::make_shared<SceneResumedSignal>();
    sig->Scene = _scenes.top()->GetSceneID();
    bus->BroadCastSignal(sig);
    if (!_scenes.empty())
    {
      _scenes.top()->Resume();
    }
  }

  void SceneManager::ReplaceScene(std::type_index scene, const SceneData &sceneData)
  {
    if (!_scenes.empty())
    {

      auto bus = SignalBus::GetInstance();
      std::shared_ptr<ScenePoppedSignal> sig = std::make_shared<ScenePoppedSignal>();
      sig->Scene = _scenes.top()->GetSceneID();
      bus->BroadCastSignal(sig);

      // Exit the current scene and pop it
      _scenes.top()->_exit();
      _scenes.pop();
    }

    // Push the new scene
    PushScene(scene, sceneData);
  }

  void SceneManager::SetQuitCallBack(QuitCallBack quitCallBack)
  {
    _quitCallBack = std::move(quitCallBack);
  }

  void SceneManager::Update(float dt)
  {
    if (_scenes.empty() && !_factories.empty())
    {
      if (_startScene != std::type_index(typeid(nullptr)))
      {
        PushScene(_startScene);
      }
      else if (_scenes.empty() && _startScene == std::type_index(typeid(nullptr)))
      {
        THROW_BASE_RUNTIME_ERROR("No starting scene specified");
      }
    }
    else if (!_scenes.empty())
    {
      // Update Current Scene
      _scenes.top()->Update(dt);
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("No Registered scene!");
    }
  }

  void SceneManager::PostUpdate()
  {
    if (!_scenes.empty())
    {
      // Check if the current scene has requested a scene change
      SceneTransition sceneTrans = _scenes.top()->GetSceneTransition();

      // Reset the scene's transisition request
      _scenes.top()->ResetSceneTransition();

      if (sceneTrans.request != SceneRequest::None)
      {
        if (sceneTrans.request == SceneRequest::Quit)
        {
          // Quit if the scene requests to quite the game
          _quitCallBack();
        }
        else if (sceneTrans.request == SceneRequest::PushNewScene)
        {
          PushScene(sceneTrans.sceneID, sceneTrans.data);
        }
        else if (sceneTrans.request == SceneRequest::ReplaceCurrentScene)
        {
          ReplaceScene(sceneTrans.sceneID, sceneTrans.data);
        }
        else if (sceneTrans.request == SceneRequest::PopCurrentScene)
        {
          PopScene();
        }
      }
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("No Registered scene!");
    }
  }

  void SceneManager::Render()
  {
    if (!_scenes.empty())
    {
      // Render current scene
      _scenes.top()->Render();
    }
  }

  void SceneManager::RegisterScene(std::type_index sceneID, FactoryCallBack factory, bool startScene)
  {
    if (_factories.find(sceneID) == _factories.end())
    {
      _factories.insert({sceneID, std::move(factory)});
      if (startScene)
      {
        _startScene = sceneID;
      }
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Registration of duplicate scene");
    }
  }

  void SceneManager::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
    if (!_scenes.empty())
    {
      _scenes.top()->_OnInputEvent(event);
    }
  }

  std::shared_ptr<const Scene> SceneManager::GetCurrentScene() const
  {
    if (!_scenes.empty())
    {
      return _scenes.top();
    }
    return nullptr;
  }
} // namespace Base
