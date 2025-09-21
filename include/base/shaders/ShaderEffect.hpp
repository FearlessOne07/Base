#pragma once
#include <raylib.h>
namespace Base
{
  class Scene;
  class ShaderEffect
  {
  protected:
    const Scene *_currentScene;
    bool _active = true;

  public:
    virtual ~ShaderEffect() = default;
    virtual void Setup(const Scene *) = 0;
    virtual void Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution) = 0;
    virtual void Update(float dt) {};

    bool IsAvtive() const
    {
      return _active;
    }
  };
} // namespace Base
