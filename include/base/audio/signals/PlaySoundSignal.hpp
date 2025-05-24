#include "base/signals/Signal.hpp"
#include <string>

namespace Base
{
  class PlaySoundSignal : public Signal
  {
  public:
    std::string soundName;
  };
} // namespace Base
// namespace Base
