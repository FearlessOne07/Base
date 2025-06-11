#include "base/renderer/RenderLayer.hpp"
#include "base/scenes/Scene.hpp"
#include "raylib.h"
#include "rlgl.h"
#include <algorithm>
#include <utility>

namespace Base
{
  RenderLayer::RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size, RenderFunction renderFunction)
    : _position(position), _size(size), _renderFunction(renderFunction), _ownerScene(ownerScene)
  {
    _renderTexture = LoadRenderTexture(_size.x, _size.y);
    _ping = LoadRenderTexture(_size.x, _size.y);
    _pong = LoadRenderTexture(_size.x, _size.y);
  }

  RenderLayer::RenderLayer(RenderLayer &&other) noexcept
    : _position(other._position), _size(other._size), _renderFunction(std::move(other._renderFunction)),
      _ownerScene(other._ownerScene), _renderTexture(other._renderTexture), _shaderChain(std::move(other._shaderChain)),
      _ping(other._ping), _pong(other._pong)
  {
    other._renderTexture.id = 0;
    other._ping.id = 0;
    other._pong.id = 0;
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
      if (_ping.id != 0)
      {
        UnloadRenderTexture(_ping);
      }
      if (_pong.id != 0)
      {
        UnloadRenderTexture(_pong);
      }

      _position = other._position;
      _size = other._size;
      _renderFunction = std::move(other._renderFunction);
      _ownerScene = other._ownerScene;
      _renderTexture = other._renderTexture;
      _shaderChain = std::move(other._shaderChain);
      _pong = other._pong;
      _ping = other._ping;

      other._renderTexture.id = 0;
      other._ping.id = 0;
      other._pong.id = 0;
    }
    return *this;
  }

  RenderLayer::~RenderLayer()
  {
    UnloadRenderTexture(_renderTexture);
    UnloadRenderTexture(_ping);
    UnloadRenderTexture(_pong);
  }

  void RenderLayer::Render()
  {
    BeginTextureMode(_renderTexture);
    ClearBackground(BLANK);
    _renderFunction();
    EndTextureMode();

    if (_shaderChain.Empty())
    {
      return;
    }

    RenderTexture2D *input = &_renderTexture;
    RenderTexture2D *output = &_ping;

    for (auto &[shaderName, pass] : _shaderChain)
    {
      BeginTextureMode(*output);
      ClearBackground(BLANK);

      auto shaderMan = _ownerScene->GetShaderManager();
      if (pass.HasDirty())
      {
        for (auto &uniform : pass.GetDirty())
        {
          shaderMan->SetUniform(shaderName, uniform.c_str(), pass.GetUniformValue(uniform));
        }
        pass.ClearDirty();
      }

      shaderMan->ActivateShader(shaderName);
      DrawTexturePro(                              //
        input->texture, {0, 0, _size.x, -_size.y}, //
        {0, 0, _size.x, _size.y}, {0, 0}, 0, WHITE //
      );
      _ownerScene->GetShaderManager()->DeactivateCurrentShader();
      EndTextureMode();

      std::swap(input, output); // Ping-pong
    }

    BeginTextureMode(_renderTexture);
    DrawTexturePro(                                                                                          //
      input->texture, {0, 0, (float)_size.x, -(float)_size.y}, {0, 0, _size.x, _size.y}, {0, 0}, 0.0f, WHITE //
    );
    EndTextureMode();
  }

  ShaderChain *RenderLayer::GetShaderChain()
  {
    return &_shaderChain;
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
