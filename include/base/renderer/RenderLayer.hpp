#pragma once
#include "base/shaders/ShaderChain.hpp"
#include "raylib.h"
#include <functional>
namespace Base
{
  class Scene;
  class RenderLayer
  {
    using RenderFunction = std::function<void()>;
    // Rendering
    RenderTexture _renderTexture;
    RenderFunction _renderFunction;

    // Spec
    Vector2 _position = {0, 0};
    Vector2 _size = {0, 0};

    // Scene
    const Scene *_ownerScene = nullptr;

    // Shaders
    bool _shaderBuffersInitialized = false;
    RenderTexture _ping;
    RenderTexture _pong;
    ShaderChain _shaderChain;

  public:
    RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size, RenderFunction renderFunction);
    RenderLayer(RenderLayer &&other) noexcept;
    RenderLayer &operator=(RenderLayer &&other) noexcept;
    ~RenderLayer();
    void Render();
    const RenderTexture *GetTexture() const;
    ShaderChain *GetShaderChain();
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
  };
} // namespace Base
