#include "base/renderer/RenderLayer.hpp"
#include "base/camera/Camera2DExt.hpp"
#include "base/scenes/Scene.hpp"
#include "raylib.h"
#include "rlgl.h"
#include <ranges>
#include <utility>

namespace Base
{
  RenderLayer::RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size, Color clearColor)
    : _position(position), _size(size), _ownerScene(ownerScene), _clearColor(clearColor)
  {
    _renderTexture = LoadRenderTexture(_size.x, _size.y);
    _ping = LoadRenderTexture(_size.x, _size.y);

    _layerCamera = Camera2DExt();
  }

  RenderLayer::RenderLayer(RenderLayer &&other) noexcept
    : _position(other._position), _size(other._size), _renderFunctions(std::move(other._renderFunctions)),
      _ownerScene(other._ownerScene), _renderTexture(other._renderTexture), _effectChain(std::move(other._effectChain)),
      _ping(other._ping)
  {
    other._renderTexture.id = 0;
    other._ping.id = 0;
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

      _position = other._position;
      _size = other._size;
      _renderFunctions = std::move(other._renderFunctions);
      _ownerScene = other._ownerScene;
      _renderTexture = other._renderTexture;
      _effectChain = std::move(other._effectChain);
      _ping = other._ping;

      other._renderTexture.id = 0;
      other._ping.id = 0;
    }
    return *this;
  }

  RenderLayer::~RenderLayer()
  {
    UnloadRenderTexture(_renderTexture);
    UnloadRenderTexture(_ping);
  }

  void RenderLayer::Render()
  {
    BeginTextureMode(_renderTexture);
    ClearBackground(_clearColor);
    BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
    auto functions = std::ranges::reverse_view(_renderFunctions);
    for (auto &function : functions)
    {
      function();
    }
    EndBlendMode();
    EndTextureMode();

    if (_effectChain.Empty())
    {
      return;
    }

    RenderTexture2D *input = &_renderTexture;
    RenderTexture2D *output = &_ping;

    for (auto &effect : _effectChain)
    {
      if (effect->IsAvtive())
      {
        effect->Apply(input, output, _size);
        std::swap(input, output); // Ping-pong
      }
    }

    if (input != &_renderTexture)
    {
      BeginTextureMode(_renderTexture);
      DrawTexturePro(                                                                                          //
        input->texture, {0, 0, (float)_size.x, -(float)_size.y}, {0, 0, _size.x, _size.y}, {0, 0}, 0.0f, WHITE //
      );
      EndTextureMode();
    }
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

  void RenderLayer::SetCameraPauseMask(const std::bitset<8> &mask)
  {
    _layerCamera.SetPauseMask(mask);
  }

  void RenderLayer::ShakeCamera(const CameraShakeConfig &config)
  {
    _layerCamera.Shake(config);
  }

  const Scene *RenderLayer::GetOwnerScene() const
  {
    return _ownerScene;
  }
  Vector2 RenderLayer::GetScreenToWorld(Vector2 position) const
  {
    return _layerCamera.GetScreenToWorld(position);
  };

  Vector2 RenderLayer::GetWorldToScreen(Vector2 position) const
  {
    return _layerCamera.GetWorldToScreen(position);
  }

  float RenderLayer::GetCameraZoom() const
  {
    return _layerCamera.GetZoom();
  }

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
    auto A = _ownerScene->GetPauseMask();
    auto B = _layerCamera.GetPauseMask();
    if (!(B.count() != 0 && (A & B) == B))
    {
      _layerCamera.Update(dt);

      for (auto &effect : _effectChain)
      {
        effect->Update(dt);
      }
    }
  }
} // namespace Base
