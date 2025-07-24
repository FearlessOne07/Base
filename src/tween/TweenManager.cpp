#include "base/tween/TweenManager.hpp"

namespace Base
{
  void TweenManager::Update(float dt)
  {
    _isUpdatingTweens = true;

    for (auto it = _tweens.begin(); it != _tweens.end();)
    {
      it->second->Update(dt);
      if (it->second->IsFinished())
      {
        it->second->OnEnd();
        it = _tweens.erase(it);
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

} // namespace Base
