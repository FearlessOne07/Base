#pragma once
#include "SceneData.hpp"
#include <cstdint>
#include <typeindex>

namespace Base
{

  enum class SceneRequest : uint8_t
  {
    None = 0,
    Quit,
    PushNewScene,
    PopCurrentScene,
    ReplaceCurrentScene
  };

  struct SceneTransition
  {
    SceneRequest request = SceneRequest::None;
    std::type_index sceneID = typeid(-1);
    SceneData data = SceneData();
  };
} // namespace Base
