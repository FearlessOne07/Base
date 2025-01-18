#pragma once
#include "base/Exports.hpp"
#include "base/SceneData.hpp"
namespace Base
{

  enum class SceneRequest
  {
    NONE = 0,
    QUIT,
    PUSH_NEW_SCENE,
    POP_CURRENT_SCENE,
    REPLACE_CURRENT_SCENE
  };

  struct BASEAPI SceneTransition
  {
    SceneRequest request = SceneRequest::NONE;
    int sceneID = -1;
    SceneData data = SceneData();
  };
} // namespace Base
