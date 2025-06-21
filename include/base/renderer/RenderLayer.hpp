#pragma once
#include "base/camera/Camera2DExt.hpp"
#include "base/shaders/ShaderChain.hpp"
#include "raylib.h"
#include <bitset>
#include <deque>
#include <functional>
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
    const Scene *_ownerScene = nullptr;

    // Shaders
    bool _shaderBuffersInitialized = false;
    RenderTexture _ping;
    ShaderChain _shaderChain;

  private:
    void Update(float dt);

  public:
    RenderLayer(const Scene *ownerScene, Vector2 position, Vector2 size, Color clearColor);
    RenderLayer(RenderLayer &&other) noexcept;
    RenderLayer &operator=(RenderLayer &&other) noexcept;
    ~RenderLayer();
    void Render();
    const RenderTexture *GetTexture() const;
    Vector2 GetSize() const;
    Vector2 GetPosition() const;
    void AddRenderFunction(const RenderFunction &);

    // Shader Chain Management
    void AddShaderPass(AssetHandle<Base::BaseShader> shaderHandle);
    void SetShaderUniform(                                                                           //
      AssetHandle<Base::BaseShader> shaderHandle, const std::string &uniformName, UniformValue value //
    );

    // Camera
    void SetCameraMode(Camera2DExtMode mode);
    void SetCameraOffset(Vector2 offset);
    void SetCameraTarget(Vector2 target);
    void SetCameraRotation(float rotation);
    void SetCameraZoom(float zoom);
    void SetCameraPauseMask(const std::bitset<8> &mask);
    void ShakeCamera(const CameraShakeConfig &config);
    Vector2 GetScreenToWorld(Vector2 position) const;
    void BeginCamera();
    void EndCamera();
  };
} // namespace Base
