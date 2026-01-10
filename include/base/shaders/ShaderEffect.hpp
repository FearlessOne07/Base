#pragma once
#include "base/util/Type.hpp"
#include <memory>

namespace Base
{
  class Scene;
  class ShaderManager;
  class FrameBuffer;
  class ShaderEffect
  {
  protected:
    std::weak_ptr<const Scene> _currentScene;
    bool _active = true;

  public:
    virtual ~ShaderEffect() = default;
    virtual void Setup(std::weak_ptr<const Scene> shaderManager) = 0;
    virtual void Apply(Ptr<FrameBuffer> input, Ptr<FrameBuffer> output, Vector2 resolution) = 0;
    virtual void Update(float dt) {};

    bool IsAvtive() const
    {
      return _active;
    }
  };
} // namespace Base
