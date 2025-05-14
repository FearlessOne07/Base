#include "base/tween/TweenManager.hpp"

namespace Base
{
  void TweenManager::Update(float dt)
  {
    for (auto it = _tweens.begin(); it != _tweens.end();)
    {
      (*it)->Update(dt);
      if ((*it)->IsFinished())
      {
        it = _tweens.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }
} // namespace Base
