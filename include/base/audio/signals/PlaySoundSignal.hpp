#pragma once
#include "base/signals/Signal.hpp"
#include <string>

namespace Base
{
  class PlaySoundSignal : public Signal
  {
  public:
    std::string soundName;
    float soundVolume = 0.f;
    float soundPan = 0.f;
  };
} // namespace Base
// namespace Base
