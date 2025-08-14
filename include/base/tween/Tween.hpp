#pragma once
#include <algorithm>
#include <functional>
#include <raylib.h>
#include <raymath.h>
#include <utility>

namespace Base
{
  enum struct TweenPriorityLevel
  {
    DEFAULT = 0,
    MEDIUM = 1,
    HIGH = 2,
  };

  class ITween
  {
  public:
    virtual ~ITween() = default;
    virtual void Update(float dt) = 0;
    virtual const void *GetTarget() const = 0;
    virtual bool IsFinished() const = 0;
    virtual void OnEnd() = 0;
    virtual TweenPriorityLevel GetProrityLevel() const = 0;
  };

  template <typename T> class Tween : public ITween
  {
    using EasingFunction = std::function<float(float)>;

  public:
  private:
    const void *_target = nullptr;
    std::function<void(T)> _setter = nullptr;
    T _startValue = 0;
    T _endValue = 0;

    float _duration = 0.f;
    float _timer = 0.f;
    EasingFunction _easingFunction = nullptr;
    std::function<void()> _onEnd;

    TweenPriorityLevel _priority;

  public:
    Tween(                                               //
      const void *target, std::function<void(T)> setter, //
      T startValue, T endValue, float duration, EasingFunction easingFunction, std::function<void()> onEnd,
      TweenPriorityLevel priority //
      )
      : _target(target), _setter(setter), _startValue(startValue), _endValue(endValue), _duration(duration),
        _easingFunction(std::move(easingFunction)), _onEnd(onEnd), _priority(priority)
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

    TweenPriorityLevel GetProrityLevel() const override
    {
      return _priority;
    };

    void OnEnd() override
    {
      if (_onEnd)
      {
        _onEnd();
      }
    }
  };
} // namespace Base
