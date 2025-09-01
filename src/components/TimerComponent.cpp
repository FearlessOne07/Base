#include "base/components/TimerComponent.hpp"

namespace Base
{

  TimerComponent::TimerComponent(float duration) : _duration(duration)
  {
  }

  float TimerComponent::GetDuration() const
  {
    return _duration;
  }

  float TimerComponent::GetElapsedTime() const
  {
    return _timer;
  }

  void TimerComponent::Advance(float dt)
  {
    _timer += dt;
  }

  void TimerComponent::ResetTimer()
  {
    _timer = 0;
  }
} // namespace Base
