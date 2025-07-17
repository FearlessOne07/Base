#pragma once
#include "TweenKey.hpp"
#include "base/tween/ITween.hpp"
#include "base/tween/Tween.hpp"
#include "base/util/Easings.hpp"
#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>

namespace Base
{

  class TweenManager
  {
  public:
    enum class EasingType : uint8_t
    {
      EASE_IN,
      EASE_OUT,
      EASE_IN_OUT
    };

    template <typename T> struct TweenSettings
    {
      T startValue = 0;
      T endValue = 0;
      float duration = 1.f;
      EasingType easingType = EasingType::EASE_OUT;
      std::function<void()> onTweenEnd = nullptr;
    };

  private:
    std::unordered_map<TweenKey, std::unique_ptr<ITween>> _tweens = {};

  public:
    void Update(float dt);

    template <typename T>
    void AddTween(                                                                              //
      const TweenKey &key, std::function<void(T)> setter, const TweenSettings<T> &tweenSettings //
    )
    {
      if (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
      {
        if (_tweens.find(key) != _tweens.end())
        {
          _tweens.erase(key);
        }

        std::function<float(float)> easingFunction = nullptr;
        switch (tweenSettings.easingType)
        {
        case EasingType::EASE_IN:
          easingFunction = Easings::EaseInCubic;
          break;
        case EasingType::EASE_OUT:
          easingFunction = Easings::EaseOutCubic;
          break;
        case EasingType::EASE_IN_OUT:
          easingFunction = Easings::EaseInOutCubic;
          break;
        }
        _tweens[key] = std::make_unique<Tween<T>>( //
          key.objectPtr, setter, tweenSettings.startValue, tweenSettings.endValue, tweenSettings.duration,
          easingFunction, tweenSettings.onTweenEnd //
        );
      }
    }
  };
} // namespace Base
