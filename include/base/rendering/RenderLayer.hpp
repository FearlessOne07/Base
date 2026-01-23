#pragma once
#include "base/camera/CameraController.hpp"
#include "base/rendering/FrameBuffer.hpp"
#include "base/rendering/Origin.hpp"
#include "base/shaders/ShaderEffect.hpp"
#include "base/shaders/ShaderEffectChain.hpp"
#include "base/util/Colors.hpp"
#include "base/util/Ref.hpp"
#include "base/util/Type.hpp"
#include <bitset>
#include <deque>
#include <functional>
#include <memory>

namespace Base
{
  class Scene;
  class SceneManager;
  class RenderLayer
  {
    friend class RenderingManager;
    using RenderFunction = std::function<void()>;

    // Rendering
    Ptr<FrameBuffer> _framebuffer;
    std::deque<RenderFunction> _renderFunctions;
    Color _clearColor = Base::Blank;

    // Camera
    CameraController _layerCamera;

    // Spec
    Vector2 _position = {0, 0};
    Vector2 _size = {0, 0};

    // Scene
    Ref<SceneManager> _sceneManager;

    // Shaders
    Ptr<FrameBuffer> _ping;
    ShaderEffectChain _effectChain;

  private:
    void Update(float dt);

  public:
    RenderLayer(Ref<SceneManager>, Vector2 position, Vector2 size, Color clearColor);
    RenderLayer(RenderLayer &&other) noexcept;
    RenderLayer &operator=(RenderLayer &&other) noexcept;
    ~RenderLayer();
    void Render();
    const Ptr<FrameBuffer> GetFramebuffer() const;
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
    void AddRenderFunction(const RenderFunction &);

    // Camera
    void SetCameraMode(CameraMode mode);
    void SetCameraTarget(Vector2 target);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);
    void SetCamerOriginPoint(Origin origin);
    void SetCameraPauseMask(const std::bitset<8> &mask);
    void ShakeCamera(const CameraShakeConfig &config);
    Vector2 GetScreenToWorld(Vector2 position) const;
    Vector2 GetWorldToScreen(Vector2 position) const;
    float GetWorldToScreen(float distance) const;
    float GetScreenToWorld(float distance) const;
    void BeginCamera();
    void EndCamera();

    float GetCameraZoom() const;

    // Shader Effect Management
    template <typename T, typename... Args> void AddShaderEffect(std::weak_ptr<Scene> ownerScene, Args &&...args)
    {
      _effectChain.AddEffect<T>(ownerScene, std::forward<Args>(args)...);
    }

    template <typename T> std::shared_ptr<T> GetShaderEffect()
    {
      return _effectChain.GetEffect<T>();
    }
  };
} // namespace Base
