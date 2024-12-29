#include "internal/scene/SceneManager.hpp"
#include "base/SceneTransition.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

namespace Base
{

void SceneManager::PushScene(int scene, SceneData sceneData)
{
  if (!_scenes.empty())
  {
    // Exit the current scene
    _scenes.top()->Exit();
  }
  // Push new scen to the stack and enter it
  _scenes.push(std::move(_factories.at(scene)()));
  _scenes.top()->Enter();
}

void SceneManager::PopScene()
{
  if (!_scenes.empty())
  {

    // Exit the current scene and pop it off the stack
    _scenes.top()->Exit();
    _scenes.pop();
  }

  // Enter the scene below it if there is one
  if (!_scenes.empty())
  {
    _scenes.top()->Enter();
  }
}

void SceneManager::ReplaceScene(int scene, SceneData sceneData)
{
  if (!_scenes.empty())
  {

    // Exit the current scene and pop it
    _scenes.top()->Exit();
    _scenes.pop();
  }

  // Push the new scene and enter it
  _scenes.push(std::move(_factories.at(scene)()));
  _scenes.top()->Enter();
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

void SceneManager::RegisterScene(int sceneID, FactoryCallBack factory)
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
    PushScene(0);
  }
}
} // namespace Base
