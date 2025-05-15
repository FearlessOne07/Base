#pragma once

#include "base/tween/ITween.hpp"
#include "base/tween/Tween.hpp"
#include "base/util/Easings.hpp"
#include <algorithm>
#include <functional>
#include <memory>
#include <type_traits>
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

  private:
    std::vector<std::unique_ptr<ITween>> _tweens = {};

  public:
    void Update(float dt);

    template <typename T>
    void AddTween( //
      void *target, std::function<void(T)> setter, T startValue, T endValue, float duration,
      EasingType type = EasingType::EASE_OUT //
    )
    {
      if (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
      {
        auto dead = std::ranges::remove_if(
          _tweens, [target](const std::unique_ptr<ITween> &t) { return t->GetTarget() == target; } //
        );
        _tweens.erase(dead.begin(), dead.end());

        std::function<float(float)> easingFunction = nullptr;

        switch (type)
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
        _tweens.emplace_back(
          std::make_unique<Tween<T>>(target, setter, startValue, endValue, duration, easingFunction));
      }
    }
  };
} // namespace Base
