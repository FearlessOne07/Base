#include "internal/scene/SceneManager.hpp"
#include "base/camera/CameraManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/ui/UIManager.hpp"
#include "base/util/Exception.hpp"
#include <utility>

namespace Base
{

  SceneManager::SceneManager( //
    EntityManager *entityManager, SystemManager *systemManager, AssetManager *assetManager,
    ParticleManager *particleManager, CameraManager *cameraManager, UIManager *uiManager, TweenManager *tweenManager //
    )
    : _entityManager(entityManager), _systemManager(systemManager), _assetManager(assetManager),
      _particleManager(particleManager), _cameraManager(cameraManager), _uiManager(uiManager),
      _tweenManager(tweenManager)
  {
  }
  void SceneManager::PushScene(std::type_index scene, const SceneData &sceneData)
  {
    if (!_scenes.empty())
    {
      // Exit the current scene
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

    _scenes.top()->SetEntityManager(_entityManager);
    _scenes.top()->SetParticleManager(_particleManager);
    _scenes.top()->SetAssetManager(_assetManager);
    _scenes.top()->SetSystemManager(_systemManager);
    _scenes.top()->SetCameraManager(_cameraManager);
    _scenes.top()->SetUIManager(_uiManager);
    _scenes.top()->SetTweenManager(_tweenManager);
    _scenes.top()->Enter(sceneData);
  }

  void SceneManager::PopScene()
  {
    if (!_scenes.empty())
    {
      // Exit the current scene and pop it off the stack
      _scenes.top()->_exit();
      _scenes.pop();
    }

    // Enter the scene below it if there is one
    if (!_scenes.empty())
    {
      _scenes.top()->Resume();
    }
  }

  void SceneManager::ReplaceScene(std::type_index scene, const SceneData &sceneData)
  {
    if (!_scenes.empty())
    {
      // Exit the current scene and pop it
      _scenes.top()->Exit();
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

      // Check if the current scene has requested a scene change
      SceneTransition sceneTrans = _scenes.top()->GetSceneTransition();

      // Reset the scene's transisition request
      _scenes.top()->ResetSceneTransition();

      if (sceneTrans.request != SceneRequest::NONE)
      {
        if (sceneTrans.request == SceneRequest::QUIT)
        {
          // Quit if the scene requests to quite the game
          _quitCallBack();
        }
        else if (sceneTrans.request == SceneRequest::PUSH_NEW_SCENE)
        {
          PushScene(sceneTrans.sceneID, sceneTrans.data);
        }
        else if (sceneTrans.request == SceneRequest::REPLACE_CURRENT_SCENE)
        {
          ReplaceScene(sceneTrans.sceneID, sceneTrans.data);
        }
        else if (sceneTrans.request == SceneRequest::POP_CURRENT_SCENE)
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
      _scenes.top()->OnInputEvent(event);
    }
  }
} // namespace Base
