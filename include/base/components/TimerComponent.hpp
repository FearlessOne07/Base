#pragma once

#include "base/components/TransitionComponent.hpp"
namespace Base
{
  class TimerComponent : public TransitionComponent
  {
  private:
    float _duration = 0;
    float _timer = 0;

  public:
    TimerComponent(float duration);
    float GetDuration() const;
    float GetElapsedTime() const;
    void Advance(float dt);
    void ResetTimer();
  };
} // namespace Base
