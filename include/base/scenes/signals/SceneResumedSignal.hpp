#pragma once

#include "base/scenes/SceneID.hpp"
#include "base/signals/Signal.hpp"
namespace Base
{
  class SceneResumedSignal : public Signal
  {
  public:
    SceneID Scene;
  };
} // namespace Base
