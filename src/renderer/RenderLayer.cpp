#include "base/renderer/RenderLayer.hpp"
#include "base/camera/Camera2DExt.hpp"
#include "base/scenes/Scene.hpp"
#include "raylib.h"
#include "rlgl.h"
#include <algorithm>
#include <ranges>
#include <utility>

namespace Base
{
  RenderLayer::RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size)
    : _position(position), _size(size), _ownerScene(ownerScene)
  {
    _renderTexture = LoadRenderTexture(_size.x, _size.y);
    _ping = LoadRenderTexture(_size.x, _size.y);
    _pong = LoadRenderTexture(_size.x, _size.y);

    _layerCamera = Camera2DExt();
  }

  RenderLayer::RenderLayer(RenderLayer &&other) noexcept
    : _position(other._position), _size(other._size), _renderFunctions(std::move(other._renderFunctions)),
      _ownerScene(other._ownerScene), _renderTexture(other._renderTexture), _shaderChain(other._shaderChain),
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
      _renderFunctions = std::move(other._renderFunctions);
      _ownerScene = other._ownerScene;
      _renderTexture = other._renderTexture;
      _shaderChain = other._shaderChain;
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
    auto functions = std::ranges::reverse_view(_renderFunctions);
    for (auto &function : functions)
    {
      function();
    }
    EndTextureMode();

    if (_shaderChain.Empty())
    {
      return;
    }

    RenderTexture2D *input = &_renderTexture;
    RenderTexture2D *output = &_ping;

    for (auto &[shaderHandle, pass] : _shaderChain)
    {
      auto shaderMan = _ownerScene->GetShaderManager();
      if (pass.HasDirty())
      {
        for (auto &uniform : pass.GetDirty())
        {
          shaderMan->SetUniform(shaderHandle, uniform.c_str(), pass.GetUniformValue(uniform));
        }
        pass.ClearDirty();
      }

      BeginTextureMode(*output);
      ClearBackground(BLANK);
      shaderMan->ActivateShader(shaderHandle);
      DrawTexturePro(                              //
        input->texture, {0, 0, _size.x, -_size.y}, //
        {0, 0, _size.x, _size.y}, {0, 0}, 0, WHITE //
      );
      shaderMan->DeactivateCurrentShader();
      EndTextureMode();

      std::swap(input, output); // Ping-pong
    }

    BeginTextureMode(_renderTexture);
    DrawTexturePro(                                                                                          //
      input->texture, {0, 0, (float)_size.x, -(float)_size.y}, {0, 0, _size.x, _size.y}, {0, 0}, 0.0f, WHITE //
    );
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

  void RenderLayer::AddRenderFunction(const RenderFunction &function)
  {
    _renderFunctions.emplace_back(std::move(function));
  }

  void RenderLayer::AddShaderPass(AssetHandle<Base::BaseShader> shaderHandle)
  {
    _shaderChain.AddShaderPass(shaderHandle);
  }
  void RenderLayer::SetShaderUniform(                                                              //
    AssetHandle<Base::BaseShader> shaderHandle, const std::string &uniformName, UniformValue value //
  )
  {
    _shaderChain.SetShaderUniform(shaderHandle, uniformName, value);
  }

  void RenderLayer::SetCameraMode(Camera2DExtMode mode)
  {
    _layerCamera.SetMode(mode);
  }

  void RenderLayer::SetCameraOffset(Vector2 offset)
  {
    _layerCamera.SetOffset(offset);
  }

  void RenderLayer::SetCameraTarget(Vector2 target)
  {
    _layerCamera.SetTarget(target);
  }

  void RenderLayer::SetCameraRotation(float rotation)
  {
    _layerCamera.SetRotation(rotation);
  }

  void RenderLayer::SetCameraZoom(float zoom)
  {
    _layerCamera.SetZoom(zoom);
  }

  void RenderLayer::ShakeCamera(const CameraShakeConfig &config)
  {
    _layerCamera.Shake(config);
  }

  Vector2 RenderLayer::GetScreenToWorld(Vector2 position) const
  {
    return _layerCamera.GetScreenToWorld(position);
  };

  void RenderLayer::BeginCamera()
  {
    _layerCamera.Begin();
  }
  void RenderLayer::EndCamera()
  {
    _layerCamera.End();
  }

  void RenderLayer::Update(float dt)
  {
    _layerCamera.Update(dt);
  }
} // namespace Base
