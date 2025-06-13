#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/audio/AudioStream.hpp"
#include "base/signals/Signal.hpp"

namespace Base
{
  class PlayAudioStreamSignal : public Signal
  {
  public:
    AssetHandle<AudioStream> streamHandle;
    float streamVolume = 1.f;
    float streamPan = 0.5f;
    bool loopStream = false;
  };
} // namespace Base
