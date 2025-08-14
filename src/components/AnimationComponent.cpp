#include "base/components/AnimationComponent.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"

namespace Base
{
  AnimationComponent::AnimationComponent(const std::string &initialAnimation) : _currentAnimation(initialAnimation)
  {
  }

  AnimationFrame &AnimationComponent::GetNextFrame()
  {
    return _animations.at(_currentAnimation)[_currentFrame];
  }

  void AnimationComponent::AddAnimation(const std::string &name, const Animation &animation)
  {
    std::string animName = Strings::ToLower(name);

    if (!_animations.contains(animName))
    {
      _animations[animName] = animation;
    }
  }

  void AnimationComponent::Advance()
  {
    _currentFrame++;
    if (_currentFrame >= _animations[_currentAnimation].size())
    {
      _currentFrame = 0;
    }
  }

  void AnimationComponent::SwitchAnimation(const std::string &name)
  {
    std::string animName = Strings::ToLower(name);

    if (_animations.contains(animName))
    {
      _currentAnimation = animName;
    }
    else
    {
      THROW_BASE_RUNTIME_ERROR("Animation " + animName + " does not exist on specified entity");
    }
  }
} // namespace Base
