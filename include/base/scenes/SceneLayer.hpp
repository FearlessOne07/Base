#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/input/InputEvent.hpp"
#include "base/renderer/RenderLayer.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "raylib.h"
#include <bitset>
#include <filesystem>
#include <memory>

namespace Base
{
  namespace fs = std::filesystem;
  class Scene;
  class SceneLayer
  {
    friend class SceneLayerStack;
    Scene *_owner = nullptr;
    RenderLayer *_renderLayer = nullptr;
    Vector2 _size = {0, 0};
    std::bitset<8> _pauseMask;
    int _layerIndex = 0;

  private:
    void _onAttach(RenderLayer *renderlayer);
    void _onDetach();
    void SetPauseMask(int index);
    void SetLayerIndex(int index);

  protected:
    Scene *GetOwner();
    RenderLayer *GetRenderLayer();
    const std::bitset<8> &GetPauseMask();

  public:
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> &event) {};

    Vector2 GetSize() const;
    Vector2 GetLayerMousePosition() const;
    float GetCameraZoom() const;

    // Camera
    void SetCameraMode(Camera2DExtMode mode);
    void SetCameraOffset(Vector2 offset);
    void SetCameraTarget(Vector2 target);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);
    void ShakeCamera(const CameraShakeConfig &config);
    Vector2 GetScreenToWorld(Vector2 position) const;
    void BeginCamera();
    void EndCamera();
    void SetCameraPauseMask();

    // Pausing
    void Pause();
    void UnPause();
    bool IsPaused();

    // Assets
    template <typename T> AssetHandle<T> GetAsset(const std::string &name) const;
    template <typename T> void LoadAsset(const fs::path &path);
    template <typename T = void> void SetSceneTransition(SceneRequest request, const SceneData &data = SceneData());
  };
} // namespace Base
