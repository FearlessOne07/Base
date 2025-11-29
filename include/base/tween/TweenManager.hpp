#pragma once
#include "TweenKey.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/tween/Tween.hpp"
#include "base/util/Easings.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Base
{
  class SceneID;
  class TweenManager
  {
  public:
    template <typename T> struct TweenSettings
    {
      T startValue = 0;
      T endValue = 0;
      float duration = 1.f;
      Easings::Type easingType = Easings::Type::EaseOut;
      std::function<void()> onTweenEnd = nullptr;
      TweenPriorityLevel priority = TweenPriorityLevel::Default;
    };

  private:
    std::unordered_map<SceneID, std::unordered_map<TweenKey, std::unique_ptr<ITween>>> _tweens;
    std::vector<std::function<void()>> _pendingTweens;
    bool _isUpdatingTweens = false;
    SceneID _currentScene;

  private:
    void UpdateCurrentScene(SceneID scene);
    void UnloadSceneTweens(SceneID scene);

  public:
    void Update(float dt);
    void Init();

    template <typename T>
    void AddTween(const TweenKey &key, std::function<void(T)> setter, const TweenSettings<T> &tweenSettings)
    {
      if (_currentScene)
      {
        if (auto it = _tweens.at(_currentScene).find(key); it != _tweens.at(_currentScene).end())
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

        if (_tweens.at(_currentScene).find(key) != _tweens.at(_currentScene).end())
        {
          _tweens.at(_currentScene).erase(key);
        }

        std::function<float(float)> easingFunction = nullptr;
        switch (tweenSettings.easingType)
        {
        case Easings::Type::EaseIn:
          easingFunction = Easings::EaseInCubic;
          break;
        case Easings::Type::EaseOut:
          easingFunction = Easings::EaseOutCubic;
          break;
        case Easings::Type::EaseInOut:
          easingFunction = Easings::EaseInOutCubic;
          break;
        }

        _tweens.at(_currentScene)[key] = std::make_unique<Tween<T>>( //
          key.objectPtr, setter, tweenSettings.startValue, tweenSettings.endValue, tweenSettings.duration,
          easingFunction, tweenSettings.onTweenEnd, tweenSettings.priority //
        );
      }
    }
  };

} // namespace Base
