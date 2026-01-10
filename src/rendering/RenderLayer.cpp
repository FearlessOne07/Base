#include "base/rendering/RenderLayer.hpp"
#include "base/camera/CameraController.hpp"
#include "base/rendering/FrameBuffer.hpp"
#include "base/scenes/Scene.hpp"
#include "internal/rendering/Renderer.hpp"
#include "internal/scene/SceneManager.hpp"
#include <ranges>
#include <utility>

namespace Base
{
  RenderLayer::RenderLayer( //
    Ref<ShaderManager> shaderManager, Ref<SceneManager> sceneManager, Vector2 position, Vector2 size,
    Color clearColor //
    )
    : _position(position), _size(size), _shaderManager(shaderManager), _sceneManager(sceneManager),
      _clearColor(clearColor)
  {
    _framebuffer = FrameBuffer::Create({.Width = _size.x, .Height = _size.y});
    _ping = FrameBuffer::Create({.Width = _size.x, .Height = _size.y});

    _layerCamera = CameraController();
  }

  RenderLayer::RenderLayer(RenderLayer &&other) noexcept
    : _position(other._position), _size(other._size), _renderFunctions(std::move(other._renderFunctions)),
      _shaderManager(other._shaderManager), _framebuffer(other._framebuffer),
      _effectChain(std::move(other._effectChain)), _ping(other._ping)
  {
    FrameBuffer::Destroy(other._framebuffer);
    FrameBuffer::Destroy(other._ping);
  }

  RenderLayer &RenderLayer::operator=(RenderLayer &&other) noexcept
  {
    if (this != &other)
    {
      // Clean up current texture
      if (_framebuffer->GetRenderId() != RenderID(0))
      {
        FrameBuffer::Destroy(_framebuffer);
      }
      if (_ping->GetRenderId() != RenderID(0))
      {
        FrameBuffer::Destroy(_ping);
      }

      _position = other._position;
      _size = other._size;
      _renderFunctions = std::move(other._renderFunctions);
      _shaderManager = other._shaderManager;
      _framebuffer = other._framebuffer;
      _effectChain = std::move(other._effectChain);
      _ping = other._ping;

      FrameBuffer::Destroy(_framebuffer);
      FrameBuffer::Destroy(_ping);
    }
    return *this;
  }

  RenderLayer::~RenderLayer()
  {
    FrameBuffer::Destroy(_framebuffer);
    FrameBuffer::Destroy(_ping);
  }

  void RenderLayer::Render()
  {
    Renderer::BeginFramebuffer(_framebuffer);
    Renderer::Clear(_clearColor);
    auto functions = std::ranges::reverse_view(_renderFunctions);
    for (auto &function : functions)
    {
      function();
    }
    Renderer::EndFramebuffer();

    if (_effectChain.Empty())
    {
      return;
    }

    Ptr<FrameBuffer> input = _framebuffer;
    Ptr<FrameBuffer> output = _ping;

    for (auto &effect : _effectChain)
    {
      if (effect->IsAvtive())
      {
        effect->Apply(input, output, _size);
        std::swap(input, output); // Ping-pong
      }
    }

    if (input != _framebuffer)
    {
      Renderer::BeginFramebuffer(_framebuffer);
      Renderer::DrawFramebuffer(input, {_size.x, _size.y}, FramebufferAttachmentIndex::Color0);
      Renderer::EndFramebuffer();
    }
  }

  const Ptr<FrameBuffer> RenderLayer::GetFramebuffer() const
  {
    return _framebuffer;
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

  void RenderLayer::SetCameraMode(CameraMode mode)
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
    auto A = _sceneManager->GetCurrentScene()->GetPauseMask();
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
