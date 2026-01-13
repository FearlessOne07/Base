#include "base/components/Component.hpp"
#include "base/util/Type.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace Base
{
  struct AnimationFrame
  {
    AnimationFrame(Vector2 frameOrgin, Vector2 frameSize, float frameDuration)
      : origin(frameOrgin), size(frameSize), duration(frameDuration)
    {
    }

    Vector2 origin{0, 0};
    Vector2 size = {0, 0};
    float duration = 0;
    float elapsed = 0.f;
  };

  using Animation = std::vector<AnimationFrame>;

  class AnimationComponent : public Component
  {
  private:
    std::unordered_map<std::string, Animation> _animations;
    float _currentFrame = 0;
    std::string _currentAnimation = "";

  public:
    AnimationFrame &GetNextFrame();
    AnimationComponent(const std::string &initialAnimation);
    void AddAnimation(const std::string &name, const Animation &animation);
    void SwitchAnimation(const std::string &name);
    void Advance();
  };
} // namespace Base
