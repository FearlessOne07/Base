#pragma once
#include "base/shaders/ShaderChain.hpp"
#include "raylib.h"
#include <deque>
#include <functional>
#include <vector>
namespace Base
{
  class Scene;
  class RenderLayer
  {
    using RenderFunction = std::function<void()>;
    // Rendering
    RenderTexture _renderTexture;
    std::deque<RenderFunction> _renderFunctions;

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
    RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size);
    RenderLayer(RenderLayer &&other) noexcept;
    RenderLayer &operator=(RenderLayer &&other) noexcept;
    ~RenderLayer();
    void Render();
    const RenderTexture *GetTexture() const;
    ShaderChain *GetShaderChain();
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
    void AddRenderFunction(const RenderFunction &);
  };
} // namespace Base
