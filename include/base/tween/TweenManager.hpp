#pragma once
#include "TweenKey.hpp"
#include "base/tween/ITween.hpp"
#include "base/tween/Tween.hpp"
#include "base/util/Easings.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

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
      TweenPriorityLevel priority = TweenPriorityLevel::DEFAULT;
    };

  private:
    std::unordered_map<TweenKey, std::unique_ptr<ITween>> _tweens;
    std::vector<std::function<void()>> _pendingTweens;
    bool _isUpdatingTweens = false;

  public:
    void Update(float dt);

    template <typename T>
    void AddTween(const TweenKey &key, std::function<void(T)> setter, const TweenSettings<T> &tweenSettings)
    {
      if (auto it = _tweens.find(key); it != _tweens.end())
      {
        TweenPriorityLevel level = it->second->GetProrityLevel();
        if (level > tweenSettings.priority)
        {
          return;
        }
      }

      if (_isUpdatingTweens)
      {
        _pendingTweens.push_back([=, this]() { AddTween<T>(key, setter, tweenSettings); });

        return;
      }

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
        key.objectPtr, setter, tweenSettings.startValue, tweenSettings.endValue, tweenSettings.duration, easingFunction,
        tweenSettings.onTweenEnd, tweenSettings.priority //
      );
    }
  };

} // namespace Base
