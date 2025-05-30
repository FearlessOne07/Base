#include "base/renderer/RenderLayer.hpp"
#include "raylib.h"

namespace Base
{
  RenderLayer::RenderLayer(Vector2 position, Vector2 size, RenderFunction renderFunction)
    : _position(position), _size(size), _renderFunction(renderFunction)
  {
    _renderTexture = LoadRenderTexture(_size.x, _size.y);
  }

  void RenderLayer::Render()
  {
    BeginTextureMode(_renderTexture);
    ClearBackground(BLANK);
    _renderFunction();
    EndTextureMode();
  }

  const RenderTexture *RenderLayer::GetTexture() const
  {
    return &_renderTexture;
  }

  Vector2 RenderLayer::GetSize() const
  {
    return _size;
  }

  Vector2 RenderLayer::GetPosition() const
  {
    return _position;
  }
} // namespace Base
