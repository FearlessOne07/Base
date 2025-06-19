#include "base/components/AnimationComponent.hpp"

namespace Base
{
  int AnimationComponent::GetCurrentFrame() const
  {
    return _currentFrame;
  }

  AnimationComponent::AnimationComponent(int frameCount, Vector2 animationStartIndex, float duration, bool loop)
    : _frameCount(frameCount), _animationStartIndex(animationStartIndex), _duration(duration), _loop(loop)
  {
    _timeBetweenFrames = _duration / (_frameCount - 1);
  }

  bool AnimationComponent::IsLoop() const
  {
    return _loop;
  }

  void AnimationComponent::Advance(float dt)
  {
    if (_frameTimer < _timeBetweenFrames)
    {
      _currentFrame++;
      if (_loop && _currentFrame >= _frameCount)
      {
        _currentFrame = 0;
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
