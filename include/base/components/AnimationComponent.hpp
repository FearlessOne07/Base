#include "base/components/Component.hpp"
#include "raylib.h"

namespace Base
{
  class AnimationComponent : public Component
  {
  private:
    int _frameCount = 0;
    Vector2 _animationStartIndex{0, 0};
    float _duration = 1.f;
    bool _loop = false;
    int _currentFrame = 0;
    float _timeBetweenFrames = 0;
    float _frameTimer = 0;

  public:
    AnimationComponent(int frameCount, Vector2 animationStartIndex, float duration, bool loop = true);
    int GetCurrentFrame() const;
    bool IsLoop() const;
    bool IsDone() const;
    void Advance(float dt);
    void Reset();
  };
} // namespace Base
