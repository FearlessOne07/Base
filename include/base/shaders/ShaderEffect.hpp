#pragma once
#include <raylib.h>
namespace Base
{
  class RenderLayer;
  class ShaderEffect
  {
  protected:
    const RenderLayer *_renderLayer;

  public:
    virtual ~ShaderEffect() = default;
    virtual void Setup(const RenderLayer *layer) = 0;
    virtual void Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution) = 0;
  };
} // namespace Base
