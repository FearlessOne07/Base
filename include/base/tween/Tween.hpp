#pragma once
#include "base/tween/ITween.hpp"
#include "raymath.h"
#include <algorithm>

namespace Base
{
  template <typename T> struct Tween : public ITween
  {
  private:
    T *_target = nullptr;
    T _startValue = 0;
    T _endValue = 0;

    float _duration = 0.f;
    float _timer = 0.f;

  public:
    Tween(T *target, T startValue, T endValue, float duration)
      : _target(target), _startValue(startValue), _endValue(endValue), _duration(duration)
    {
    }
    void Update(float dt) override
    {
      _timer += dt;

      float lifePoint = std::clamp<float>(1.f - (_timer / _duration), 0, 1);

      if (_target)
      {
        *_target = static_cast<T>(Lerp(_startValue, _endValue, lifePoint));
      }
    }

    void *GetTarget() const override
    {
      return static_cast<void *>(_target);
    }

    bool IsFinished() const override
    {
      return _timer >= _duration;
    }
  };
} // namespace Base
