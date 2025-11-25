#include "base/tween/TweenManager.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/signals/SignalBus.hpp"

namespace Base
{
  void TweenManager::Update(float dt)
  {
    _isUpdatingTweens = true;

    for (auto it = _tweens.at(_currentScene).begin(); it != _tweens.at(_currentScene).end();)
    {
      it->second->Update(dt);
      if (it->second->IsFinished())
      {
        it->second->OnEnd();
        it = _tweens.at(_currentScene).erase(it);
      }
      else
      {
        ++it;
      }
    }
    _isUpdatingTweens = false;

    // Flush pending tweens after main update loop
    for (auto &addTween : _pendingTweens)
      addTween();
    _pendingTweens.clear();
  }

  void TweenManager::Init()
  {
    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> sig) {
      auto scenePushed = std::static_pointer_cast<ScenePushedSignal>(sig);
      UpdateCurrentScene(scenePushed->Scene);
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> sig) {
      auto sceneResumed = std::static_pointer_cast<SceneResumedSignal>(sig);
      UpdateCurrentScene(sceneResumed->Scene);
    });

    bus->SubscribeSignal<ScenePoppedSignal>([this](std::shared_ptr<Signal> signal) {
      auto scenePopped = std::static_pointer_cast<ScenePoppedSignal>(signal);
      UnloadSceneTweens(scenePopped->Scene);
    });
  }

  void TweenManager::UnloadSceneTweens(SceneID scene)
  {
    _tweens.erase(_currentScene);
    _currentScene = {};
  }

  void TweenManager::UpdateCurrentScene(SceneID scene)
  {
    _currentScene = scene;
    _tweens[_currentScene];
  }
} // namespace Base
