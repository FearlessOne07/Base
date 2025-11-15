#pragma once
#include "base/camera/Camera2DExt.hpp"
#include "base/shaders/ShaderEffect.hpp"
#include "base/shaders/ShaderEffectChain.hpp"
#include "base/shaders/ShaderManager.hpp"
#include "raylib.h"
#include <bitset>
#include <deque>
#include <functional>
#include <memory>

namespace Base
{
  class Scene;
  class RenderLayer
  {
    friend class Renderer;
    using RenderFunction = std::function<void()>;

    // Rendering
    RenderTexture _renderTexture;
    std::deque<RenderFunction> _renderFunctions;
    Color _clearColor = BLANK;

    // Camera
    Camera2DExt _layerCamera;

    // Spec
    Vector2 _position = {0, 0};
    Vector2 _size = {0, 0};

    // Scene
    Ref<ShaderManager> _shaderManager;

    // Shaders
    RenderTexture _ping;
    ShaderEffectChain _effectChain;

  private:
    void Update(float dt);

  public:
    RenderLayer(Ref<ShaderManager> shaderManager, Vector2 position, Vector2 size, Color clearColor);
    RenderLayer(RenderLayer &&other) noexcept;
    RenderLayer &operator=(RenderLayer &&other) noexcept;
    ~RenderLayer();
    void Render();
    const RenderTexture *GetTexture() const;
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
    void AddRenderFunction(const RenderFunction &);
    const Scene *GetOwnerScene() const;

    // Camera
    void SetCameraMode(Camera2DExtMode mode);
    void SetCameraOffset(Vector2 offset);
    void SetCameraTarget(Vector2 target);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);
    void SetCameraPauseMask(const std::bitset<8> &mask);
    void ShakeCamera(const CameraShakeConfig &config);
    Vector2 GetScreenToWorld(Vector2 position) const;
    Vector2 GetWorldToScreen(Vector2 position) const;
    void BeginCamera();
    void EndCamera();

    float GetCameraZoom() const;

    // Shader Effect Management
    template <typename T, typename... Args> void AddShaderEffect(Args &&...args)
    {
      _effectChain.AddEffect<T>(_shaderManager, std::forward<Args>(args)...);
    }

    template <typename T> std::shared_ptr<T> GetShaderEffect()
    {
      return _effectChain.GetEffect<T>();
    }
  };
} // namespace Base
