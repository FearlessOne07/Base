#pragma once
#include "core/scene/SceneTransition.hpp"
#include "raylib.h"

class Scene
{
private:
  SceneTransition _sceneTransition = SceneTransition();
  Color _fillColor = BLACK;

protected: // Inclass Access
  void SetSceneClearColor(Color color)
  {
    _fillColor = color;
  }
  void SetSceneTransition(SceneTransition transition)
  {
    _sceneTransition = transition;
  }
  void Clear() const
  {
    ClearBackground(_fillColor);
  };

protected: // Virtual
  virtual void GetInput() = 0;

public:
  virtual ~Scene()
  {
  }
  virtual void Update(float dt) = 0;
  virtual void Enter(SceneData sceneData = SceneData()) = 0;
  virtual void Render() = 0;
  virtual void Exit() = 0;

  SceneTransition GetSceneTransition() const
  {
    return _sceneTransition;
  }
  void ResetSceneTransition()
  {
    _sceneTransition = {SceneRequest::NONE, -1};
  }
};
