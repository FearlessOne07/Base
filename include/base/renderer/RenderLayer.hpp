#pragma once
#include "raylib.h"
#include <functional>
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

  public:
    RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size, RenderFunction renderFunction);
    RenderLayer(RenderLayer &&other) noexcept;
    RenderLayer &operator=(RenderLayer &&other) noexcept;
    ~RenderLayer();
    void Render();
    const RenderTexture *GetTexture() const;
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
  };
} // namespace Base
