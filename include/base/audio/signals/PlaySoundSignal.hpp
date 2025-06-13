#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/signals/Signal.hpp"
#include "raylib.h"

namespace Base
{
  class PlaySoundSignal : public Signal
  {
  public:
    AssetHandle<Sound> soundHandle;
    float soundVolume = 1.f;
    float soundPan = 0.5f;
  };
} // namespace Base
// namespace Base
