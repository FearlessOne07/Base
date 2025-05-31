#include "base/renderer/RenderLayer.hpp"
#include "base/renderer/ShaderBuffer.hpp"
#include "base/scenes/Scene.hpp"
#include "raylib.h"
#include <algorithm>
#include <utility>

namespace Base
{
  RenderLayer::RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size, RenderFunction renderFunction)
    : _position(position), _size(size), _renderFunction(renderFunction), _ownerScene(ownerScene)
  {
    _renderTexture = LoadRenderTexture(_size.x, _size.y);
  }

  RenderLayer::RenderLayer(RenderLayer &&other) noexcept
    : _position(other._position), _size(other._size), _renderFunction(std::move(other._renderFunction)),
      _ownerScene(other._ownerScene), _renderTexture(other._renderTexture), _shaderChain(std::move(other._shaderChain))
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
      _shaderChain = std::move(other._shaderChain);

      other._renderTexture.id = 0;
    }
    return *this;
  }

  RenderLayer::~RenderLayer()
  {
    UnloadRenderTexture(_renderTexture);
  }

  void RenderLayer::Render(ShaderBuffer &shaderBuffer)
  {
    BeginTextureMode(_renderTexture);
    ClearBackground(BLANK);
    _renderFunction();
    EndTextureMode();

    if (_shaderChain.empty())
    {
      return;
    }

    RenderTexture2D *input = &_renderTexture;
    RenderTexture2D *output = &shaderBuffer.ping;

    for (size_t i = 0; i < _shaderChain.size(); ++i)
    {
      BeginTextureMode(*output);
      ClearBackground(BLANK);

      BeginShaderMode(*_shaderChain[i]);
      DrawTexturePro(                              //
        input->texture, {0, 0, _size.x, -_size.y}, //
        {0, 0, _size.x, _size.y}, {0, 0}, 0, WHITE //
      );
      EndShaderMode();
      EndTextureMode();

      std::swap(input, output); // Ping-pong
    }

    BeginTextureMode(_renderTexture);
    DrawTexturePro(                                                                                          //
      input->texture, {0, 0, (float)_size.x, -(float)_size.y}, {0, 0, _size.x, _size.y}, {0, 0}, 0.0f, WHITE //
    );
    EndTextureMode();
  }

  void RenderLayer::SetShaderChain(const std::vector<std::shared_ptr<Shader>> &chain)
  {
    _shaderChain = std::move(chain);
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
