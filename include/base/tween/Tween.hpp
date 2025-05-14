#pragma once
#include "base/tween/ITween.hpp"
#include "raymath.h"
#include <algorithm>
#include <functional>

namespace Base
{
  template <typename T> struct Tween : public ITween
  {
  private:
    void *_target = nullptr;
    std::function<void(T)> _setter = nullptr;
    T _startValue = 0;
    T _endValue = 0;

    float _duration = 0.f;
    float _timer = 0.f;

  public:
    Tween(                                         //
      void *target, std::function<void(T)> setter, //
      T startValue, T endValue,
      float duration //
      )
      : _target(target), _setter(setter), _startValue(startValue), _endValue(endValue), _duration(duration)
    {
    }
    void Update(float dt) override
    {
      _timer += dt;

      float lifePoint = std::clamp<float>(_timer / _duration, 0, 1);

      if (_setter && _target)
      {
        _setter(static_cast<T>(Lerp(_startValue, _endValue, lifePoint)));
      }
    }

    void *GetTarget() const override
    {
      return _target;
    }

    bool IsFinished() const override
    {
      return _timer >= _duration;
    }
  };
} // namespace Base
