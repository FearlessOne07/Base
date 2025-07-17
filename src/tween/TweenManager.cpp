#include "base/tween/TweenManager.hpp"

namespace Base
{
  void TweenManager::Update(float dt)
  {
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
  }
} // namespace Base
