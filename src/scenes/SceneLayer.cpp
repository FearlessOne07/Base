#include "base/scenes/SceneLayer.hpp"
#include "base/scenes/Scene.hpp"
#include "base/util/Type.hpp"
#include "internal/rendering/Renderer.hpp"
#include <memory>

namespace Base
{
  std::shared_ptr<Scene> SceneLayer::GetOwner()
  {
    return _owner.lock();
  }

  const std::bitset<8> &SceneLayer::GetPauseMask()
  {
    return _pauseMask;
  }

  void SceneLayer::_onAttach(Ref<RenderLayer> renderLayer)
  {
    _renderLayer = renderLayer;
    _size = _renderLayer->GetSize();
    _renderLayer->AddRenderFunction([this]() { this->Render(); });
    OnAttach();
  }

  void SceneLayer::SetPauseMask(int index)
  {
    _pauseMask.set(index);
  }

  void SceneLayer::Pause()
  {
    GetOwner()->PauseLayer(_layerIndex);
  }

  void SceneLayer::UnPause()
  {
    GetOwner()->UnPauseLayer(_layerIndex);
  }

  bool SceneLayer::IsPaused()
  {
    return GetOwner()->IsLayerPaused(_layerIndex);
  }

  void SceneLayer::SetLayerIndex(int index)
  {
    _layerIndex = index;
  }

  void SceneLayer::_onDetach()
  {
    OnDetach();
  }

  Vector2 SceneLayer::GetSize() const
  {
    return _size;
  }

  float SceneLayer::GetCameraZoom() const
  {
    return _renderLayer->GetCameraZoom();
  }

  Vector2 SceneLayer::GetLayerCameraMousePosition() const
  {
    auto windowSize = Renderer::GetWindowSize();
    float scale = std::min( //
      windowSize.x / _size.x,
      windowSize.y / _size.y //
    );
    float marginX = (windowSize.x - (_size.x * scale)) / 2;
    float marginY = (windowSize.y - (_size.y * scale)) / 2;

    return GetScreenToWorld({
      (Renderer::GetWindowMousePosition().x - marginX) / scale,
      (Renderer::GetWindowMousePosition().y - marginY) / scale,
    });
  }

  Vector2 SceneLayer::GetLayerMousePosition() const
  {
    Vector2 windowSize = Renderer::GetWindowSize();
    float scale = std::min( //
      (float)windowSize.x / _size.x,
      (float)windowSize.y / _size.y //
    );
    float marginX = (windowSize.x - (_size.x * scale)) / 2;
    float marginY = (windowSize.y - (_size.y * scale)) / 2;
    return {
      (Renderer::GetWindowMousePosition().x - marginX) / scale,
      (Renderer::GetWindowMousePosition().y - marginY) / scale,
    };
  }

  void SceneLayer::SetCameraPauseMask()
  {
    _renderLayer->SetCameraPauseMask(_pauseMask);
  }

  void SceneLayer::SetCameraMode(CameraMode mode)
  {
    _renderLayer->SetCameraMode(mode);
  }

  void SceneLayer::SetCameraOffset(Vector2 offset)
  {
    _renderLayer->SetCameraOffset(offset);
  }

  void SceneLayer::SetCameraTarget(Vector2 target)
  {
    _renderLayer->SetCameraTarget(target);
  }

  void SceneLayer::SetCameraRotation(float rotation)
  {
    _renderLayer->SetCameraRotation(rotation);
  }

  void SceneLayer::SetCameraZoom(float zoom)
  {
    _renderLayer->SetCameraZoom(zoom);
  }

  void SceneLayer::ShakeCamera(const CameraShakeConfig &config)
  {
    _renderLayer->ShakeCamera(config);
  }

  Vector2 SceneLayer::GetScreenToWorld(Vector2 position) const
  {
    return _renderLayer->GetScreenToWorld(position);
  };

  Vector2 SceneLayer::GetWorldToScreen(Vector2 position) const
  {
    return _renderLayer->GetWorldToScreen(position);
  };

  float SceneLayer::GetWorldToScreen(float distance) const
  {
    return _renderLayer->GetWorldToScreen(distance);
  }

  float SceneLayer::GetScreenToWorld(float distance) const
  {
    return _renderLayer->GetScreenToWorld(distance);
  }

  void SceneLayer::BeginCamera()
  {
    _renderLayer->BeginCamera();
  }

  void SceneLayer::EndCamera()
  {
    _renderLayer->EndCamera();
  }
} // namespace Base
