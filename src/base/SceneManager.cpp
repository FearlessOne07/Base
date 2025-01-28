#include "internal/scene/SceneManager.hpp"
#include "base/EntityManager.hpp"
#include "base/Scene.hpp"
#include "base/SceneTransition.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

namespace Base
{

  SceneManager::SceneManager(EntityManager *entityManager, SystemManager *systemManager, AssetManager *assetManager)
    : _entityManager(entityManager), _systemManager(systemManager), _assetManager(assetManager)
  {
  }

  void SceneManager::PushScene(std::type_index scene, SceneData sceneData)
  {
    if (!_scenes.empty())
    {
      // Exit the current scene
      _scenes.top()->Exit(_systemManager, _assetManager);
    }
    // Push new scen to the stack and enter it
    _scenes.push(_factories.at(scene)());
    _scenes.top()->SetEntityManager(_entityManager);
    _scenes.top()->Enter(_systemManager, _assetManager, sceneData);
  }

  void SceneManager::PopScene()
  {
    if (!_scenes.empty())
    {
      // Exit the current scene and pop it off the stack
      _scenes.top()->Exit(_systemManager, _assetManager);
      _scenes.pop();
    }

    // Enter the scene below it if there is one
    if (!_scenes.empty())
    {
      _scenes.top()->Enter(_systemManager, _assetManager);
    }
  }

  void SceneManager::ReplaceScene(std::type_index scene, SceneData sceneData)
  {
    if (!_scenes.empty())
    {
      // Exit the current scene and pop it
      _scenes.top()->Exit(_systemManager, _assetManager);
      _scenes.pop();
    }

    // Push the new scene and enter it
    _scenes.push(_factories.at(scene)());
    _scenes.top()->Enter(_systemManager, _assetManager, sceneData);
  }

  void SceneManager::SetQuitCallBack(QuitCallBack quitCallBack)
  {
    _quitCallBack = quitCallBack;
  }

  void SceneManager::Update(float dt)
  {
    if (!_scenes.empty())
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
          std::cout << "Quiting!\n";
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
      throw std::runtime_error("No scene in scene stack");
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

  void SceneManager::RegisterScene(std::type_index sceneID, FactoryCallBack factory)
  {
    if (_factories.find(sceneID) == _factories.end())
    {
      _factories.insert({sceneID, std::move(factory)});
    }
    else
    {
      throw std::runtime_error("Registration of duplicate scene");
    }

    if (_scenes.empty())
    {
      std::type_index first = typeid(0);
      for (auto &[id, factory] : _factories)
      {
        first = id;
        break;
      }
      PushScene(first);
    }
  }
} // namespace Base
