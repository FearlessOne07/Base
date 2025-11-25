#pragma once
#include <memory>
#include <raylib.h>
namespace Base
{
  class Scene;
  class ShaderManager;
  class ShaderEffect
  {
  protected:
    std::weak_ptr<const Scene> _currentScene;
    bool _active = true;

  public:
    virtual ~ShaderEffect() = default;
    virtual void Setup(std::weak_ptr<const Scene> shaderManager) = 0;
    virtual void Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution) = 0;
    virtual void Update(float dt) {};

    bool IsAvtive() const
    {
      return _active;
    }
  };
} // namespace Base
