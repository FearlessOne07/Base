#pragma once
#include "raylib.h"
#include <functional>
#include <memory>
#include <vector>
namespace Base
{
  class Scene;
  class RenderLayer
  {
    using RenderFunction = std::function<void()>;
    RenderTexture _renderTexture;
    Vector2 _position = {0, 0};
    Vector2 _size = {0, 0};
    RenderFunction _renderFunction;
    const Scene *_ownerScene = nullptr;

    // Shaders
    bool _shaderBuffersInitialized = false;
    RenderTexture _ping;
    RenderTexture _pong;
    std::vector<std::shared_ptr<Shader>> _shaderChain;

  public:
    RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size, RenderFunction renderFunction);
    RenderLayer(RenderLayer &&other) noexcept;
    RenderLayer &operator=(RenderLayer &&other) noexcept;
    ~RenderLayer();
    void Render();
    const RenderTexture *GetTexture() const;
    void SetShaderChain(const std::vector<std::shared_ptr<Shader>> &);
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
  };
} // namespace Base
