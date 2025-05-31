#pragma once

#include "base/scenes/Scene.hpp"
#include "base/signals/Signal.hpp"
namespace Base
{
  class SceneSuspendedSignal : public Signal
  {
  public:
    const Scene *scene = nullptr;
  };
} // namespace Base
