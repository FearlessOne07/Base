#include "base/scenes/SceneLayer.inl"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "raylib.h"

namespace Base
{
  Scene *SceneLayer::GetOwner()
  {
    return _owner;
  }

  RenderLayer *SceneLayer::GetRenderLayer()
  {
    return _renderLayer;
  }

  const std::bitset<8> &SceneLayer::GetPauseMask()
  {
    return _pauseMask;
  }

  void SceneLayer::_onAttach(RenderLayer *renderLayer)
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
  Vector2 SceneLayer::GetLayerMousePosition() const
  {
    auto windowWidth = static_cast<float>(GetScreenWidth());
    auto windowHeight = static_cast<float>(GetScreenHeight());
    float scale = std::min( //
      (float)windowWidth / _size.x,
      (float)windowHeight / _size.y //
    );
    float marginX = (windowWidth - (_size.x * scale)) / 2;
    float marginY = (windowHeight - (_size.y * scale)) / 2;
    return {
      (GetMousePosition().x - marginX) / scale,
      (GetMousePosition().y - marginY) / scale,
    };
  }
  void SceneLayer::SetCameraPauseMask()
  {
    _renderLayer->SetCameraPauseMask(_pauseMask);
  }

  void SceneLayer::SetCameraMode(Camera2DExtMode mode)
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

  void SceneLayer::BeginCamera()
  {
    _renderLayer->BeginCamera();
  }

  void SceneLayer::EndCamera()
  {
    _renderLayer->EndCamera();
  }
} // namespace Base
