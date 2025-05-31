#include "base/renderer/RenderLayer.hpp"
#include "base/scenes/Scene.hpp"
#include "raylib.h"

namespace Base
{
  RenderLayer::RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size, RenderFunction renderFunction)
    : _position(position), _size(size), _renderFunction(renderFunction), _ownerScene(ownerScene)
  {
    _renderTexture = LoadRenderTexture(_size.x, _size.y);
  }

  RenderLayer::RenderLayer(RenderLayer &&other) noexcept
    : _position(other._position), _size(other._size), _renderFunction(std::move(other._renderFunction)),
      _ownerScene(other._ownerScene), _renderTexture(other._renderTexture) // Steal the texture
  {
    other._renderTexture.id = 0;
  }

  RenderLayer &RenderLayer::operator=(RenderLayer &&other) noexcept
  {
    if (this != &other)
    {
      // Clean up current texture
      if (_renderTexture.id != 0)
      {
        UnloadRenderTexture(_renderTexture);
      }

      _position = other._position;
      _size = other._size;
      _renderFunction = std::move(other._renderFunction);
      _ownerScene = other._ownerScene;
      _renderTexture = other._renderTexture;

      other._renderTexture.id = 0;
    }
    return *this;
  }

  RenderLayer::~RenderLayer()
  {
    UnloadRenderTexture(_renderTexture);
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
