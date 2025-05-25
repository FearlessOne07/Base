#pragma once
#include "base/signals/Signal.hpp"
#include <string>

namespace Base
{
  class PlayAudioStreamSignal : public Signal
  {
  public:
    std::string streamName;
    float streamVolume = 1.f;
    float streamPan = 0.5f;
    bool loopStream = false;
  };
} // namespace Base
