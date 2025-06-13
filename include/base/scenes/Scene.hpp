#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/camera/CameraManager.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/renderer/Renderer.hpp"
#include "base/scenes/SceneLayerStack.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/shaders/ShaderManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include "raylib.h"
#include <memory>
#include <typeindex>

namespace Base
{
  class EntityManager;
  class SystemManager;
  class AssetManager;
  class Scene
  {
  private:
    friend class SceneManager;
    [[nodiscard]] SceneTransition GetSceneTransition() const;
    void SetEntityManager(EntityManager *);
    void SetParticleManager(ParticleManager *);
    void SetAssetManager(AssetManager *);
    void SetSystemManager(SystemManager *);
    void SetCameraManager(CameraManager *);
    void SetUIManager(UIManager *);
    void SetTweenManager(TweenManager *);
    void SetRenderer(Renderer *);
    void SetShaderManager(ShaderManager *);
    void _setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data = SceneData());
    void ResetSceneTransition();

    // Core
    void Render();
    void Update(float dt);

    // Template Methods
    void _exit();

    struct SceneState
    {
      SceneTransition sceneTransition = SceneTransition();
      Renderer *renderer = nullptr;
      EntityManager *entityManager = nullptr;
      ParticleManager *particleManager = nullptr;
      AssetManager *assetManager = nullptr;
      SystemManager *systemManager = nullptr;
      CameraManager *cameraManager = nullptr;
      UIManager *uiManager = nullptr;
      TweenManager *tweenManager = nullptr;
      ShaderManager *shaderManager = nullptr;
      Color clearColor = BLACK;
    };

    std::unique_ptr<SceneState> _state;
    SceneLayerStack _layerStack;
    std::unordered_map<std::string, AssetHandle<void>> _assets;

    // Private Getters
    [[nodiscard]] Renderer *GetRenderer() const;

  protected:
    void SetClearColor(Color color);
    RenderLayer *AddRenderLayer(Vector2 size);

  public:
    Scene();
    virtual ~Scene() = default;
    virtual void Enter(SceneData sceneData = SceneData()) = 0;
    virtual void Exit() = 0;
    virtual void Resume();
    virtual void Suspend();
    virtual void OnInputEvent(std::shared_ptr<InputEvent> event);

    Color GetClearColor() const;
    [[nodiscard]] EntityManager *GetEntityManager() const;
    [[nodiscard]] SystemManager *GetSystemManager() const;
    [[nodiscard]] ParticleManager *GetParticleManager() const;
    [[nodiscard]] AssetManager *GetAssetManager() const;
    [[nodiscard]] CameraManager *GetCameraManager() const;
    [[nodiscard]] UIManager *GetUIManager() const;
    [[nodiscard]] TweenManager *GetTweenManager() const;
    [[nodiscard]] ShaderManager *GetShaderManager() const;
    [[nodiscard]] SceneLayerStack &GetLayerStack();

    template <typename T = void> void SetSceneTransition(SceneRequest request, const SceneData &data = SceneData())
    {
      _setSceneTransition(typeid(T), request, data);
    }

    // Asset Management
    template <typename T> AssetHandle<T> GetAsset(const std::string &name) const
    {
      if (_assets.contains(name))
      {
        return AssetHandle<T>::Cast(_assets.at(name));
      }
      else
      {
        THROW_BASE_RUNTIME_ERROR("Asset " + name + " not loaded");
      }
    }

    template <typename T> AssetHandle<T> GetGlobalAsset(const std::string &name)
    {
      return GetAssetManager()->GetAsset<T>(name);
    }

    template <typename T> void LoadAsset(const fs::path &path)
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      _assets[name] = GetAssetManager()->LoadAsset<T>(path, false);
    }
  };
} // namespace Base
