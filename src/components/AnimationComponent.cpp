#include "base/components/AnimationComponent.hpp"

namespace Base
{
  int AnimationComponent::GetCurrentFrame() const
  {
    return _currentFrame;
  }

  AnimationComponent::AnimationComponent(int frameCount, Vector2 animationStartIndex, float frameTime, bool loop)
    : _frameCount(frameCount), _animationStartIndex(animationStartIndex), _frameTime(frameTime), _loop(loop)
  {
    _currentFrame = _animationStartIndex.x;
  }

  bool AnimationComponent::IsLoop() const
  {
    return _loop;
  }

  void AnimationComponent::Advance(float dt)
  {
    if (_frameTimer >= _frameTime)
    {
      _frameTimer = 0;
      _currentFrame++;
      if (_loop && _currentFrame >= _animationStartIndex.x + _frameCount)
      {
        _currentFrame = _animationStartIndex.x;
      }
    }
    else
    {
      _frameTimer += dt;
    }
  }

  void AnimationComponent::Reset()
  {
    _currentFrame = 0;
  }

  bool AnimationComponent::IsDone() const
  {
    return _currentFrame >= _frameCount;
  }
} // namespace Base
