#pragma once

#include "base/tween/ITween.hpp"
#include "base/tween/Tween.hpp"
#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>
namespace Base
{
  class TweenManager
  {
    std::vector<std::unique_ptr<ITween>> _tweens = {};

  public:
    void Update(float dt);

    template <typename T>
    void AddTween(void *target, std::function<void(T)> setter, T startValue, T endValue, float duration)
    {
      if (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
      {
        auto dead = std::ranges::remove_if(
          _tweens, [target](const std::unique_ptr<ITween> &t) { return t->GetTarget() == target; } //
        );
        _tweens.erase(dead.begin(), dead.end());
        _tweens.emplace_back(std::make_unique<Tween<T>>(target, setter, startValue, endValue, duration));
      }
    }
  };
} // namespace Base
