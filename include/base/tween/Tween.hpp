#pragma once
#include "base/tween/ITween.hpp"
#include <algorithm>
#include <functional>
#include <raylib.h>
#include <raymath.h>
#include <utility>

namespace Base
{
  template <typename T> struct Tween : public ITween
  {
    using EasingFunction = std::function<float(float)>;

  private:
    const void *_target = nullptr;
    std::function<void(T)> _setter = nullptr;
    T _startValue = 0;
    T _endValue = 0;

    float _duration = 0.f;
    float _timer = 0.f;
    EasingFunction _easingFunction = nullptr;
    std::function<void()> _onEnd;

  public:
    Tween(                                                                                                 //
      const void *target, std::function<void(T)> setter,                                                   //
      T startValue, T endValue, float duration, EasingFunction easingFunction, std::function<void()> onEnd //
      )
      : _target(target), _setter(setter), _startValue(startValue), _endValue(endValue), _duration(duration),
        _easingFunction(std::move(easingFunction)), _onEnd(onEnd)
    {
    }

    T TweenLerp(T start, T end, float t);
    void Update(float dt) override
    {
      _timer += dt;

      float lifePoint = std::clamp<float>(_timer / _duration, 0, 1);
      if (_easingFunction)
      {
        lifePoint = _easingFunction(lifePoint);
      }

      if (_setter && _target)
      {
        _setter(TweenLerp(_startValue, _endValue, lifePoint));
      }
    }

    const void *GetTarget() const override
    {
      return _target;
    }

    bool IsFinished() const override
    {
      return _timer >= _duration;
    }

    void OnEnd() override
    {
      if (_onEnd)
      {
        _onEnd();
      }
    }
  };
} // namespace Base
