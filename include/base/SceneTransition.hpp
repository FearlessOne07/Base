#pragma once
#include "base/SceneData.hpp"

enum class SceneRequest
{
  NONE = 0,
  QUIT,
  PUSH_NEW_SCENE,
  POP_CURRENT_SCENE,
  REPLACE_CURRENT_SCENE
};

struct SceneTransition
{
  SceneRequest request = SceneRequest::NONE;
  int sceneID = -1;
  SceneData data = SceneData();
};
