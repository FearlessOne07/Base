#pragma once
#include "SceneData.hpp"
#include <cstdint>
#include <typeindex>

namespace Base
{

  enum class SceneRequest : uint8_t
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
    std::type_index sceneID = typeid(-1);
    SceneData data = SceneData();
  };
} // namespace Base
