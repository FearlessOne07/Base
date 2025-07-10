
#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/audio/AudioStream.hpp"
#include "base/signals/Signal.hpp"

namespace Base
{
  class StopAudioStreamSignal : public Signal
  {
  public:
    AssetHandle<AudioStream> streamHandle;
  };

} // namespace Base
