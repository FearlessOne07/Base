#pragma once
#include "base/assets/AssetManager.hpp"
#include "base/scenes/SceneLayerStack.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/scenes/SharedSceneDataStore.hpp"
#include "base/shaders/ShaderEffectChain.hpp"
#include "base/shaders/ShaderManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
#include "base/util/Strings.hpp"
#include "raylib.h"
#include <bitset>
#include <filesystem>
#include <memory>
#include <unordered_map>

namespace Base
{

  namespace fs = std::filesystem;
  class EntityManager;
  class SystemManager;
  class AssetManager;
  class ParticleManager;
  class RenderLayer;
  class Renderer;

  class Scene : public std::enable_shared_from_this<Scene>
  {

  private:
    friend class SceneManager;
    void Init();
    void SetEntityManager(Ref<EntityManager>);
    void SetSceneID(SceneID id);
    void SetParticleManager(Ref<ParticleManager>);
    void SetAssetManager(Ref<AssetManager>);
    void SetSystemManager(Ref<SystemManager>);
    void SetUIManager(Ref<UIManager>);
    void SetTweenManager(Ref<TweenManager>);
    void SetRenderer(Ref<Renderer>);
    void SetShaderManager(Ref<ShaderManager>);
    void _setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data = SceneData());
    void ResetSceneTransition();

    // Core
    void Render();
    void Update(float dt);

    // Template Methods
    void _exit();

    struct SceneState
    {
      SceneID sceneID;
      SceneTransition sceneTransition = SceneTransition();
      Ref<Renderer> renderer;
      Ref<EntityManager> entityManager;
      Ref<ParticleManager> particleManager;
      Ref<AssetManager> assetManager;
      Ref<SystemManager> systemManager;
      Ref<UIManager> uiManager;
      Ref<TweenManager> tweenManager;
      Ref<ShaderManager> shaderManager;
      Color clearColor = BLACK;
      SharedSceneDataStore<void> sharedData;
    };

    std::unique_ptr<SceneState> _state;
    SceneLayerStack _layerStack;
    std::unordered_map<std::string, AssetHandle<void>> _assets;

    // Private Getters
    [[nodiscard]] Ref<Renderer> GetRenderer() const;
    [[nodiscard]] Ref<AssetManager> GetAssetManager() const;
    [[nodiscard]] const SceneTransition &GetSceneTransition() const;

    // Pause
    std::bitset<8> _pauseMask;

    // Scene Store

  protected:
    // Rendering
    void SetClearColor(Color color);
    Ref<RenderLayer> AddRenderLayer(Vector2 size, Color clearColor = BLANK);

    ShaderEffectChain _postProcessingEffects;

    // Shader Effect Management
    template <typename T, typename... Args> void AddPostProcessingEffect(Args &&...args)
    {
      _postProcessingEffects.AddEffect<T>(this, std::forward<Args>(args)...);
    }

    // Shared Data
    template <typename T> void InitSharedData()
    {
      auto data = SharedSceneDataStore<T>();
      data.Init();

      _state->sharedData = data;
    }

  public:
    Scene() = default;
    virtual ~Scene() = default;
    virtual void Enter(const SceneData &sceneData = SceneData()) = 0;
    virtual void Exit() = 0;
    virtual void Resume();
    virtual void Suspend();
    virtual void OnInputEvent(std::shared_ptr<InputEvent> event);
    void PauseLayer(int layerIndex);
    void UnPauseLayer(int layerIndex);
    bool IsLayerPaused(int layerIndex);

    void _OnInputEvent(std::shared_ptr<InputEvent> event);

    Color GetClearColor() const;
    [[nodiscard]] SceneID GetSceneID() const;
    [[nodiscard]] Ref<EntityManager> GetEntityManager() const;
    [[nodiscard]] Ref<SystemManager> GetSystemManager() const;
    [[nodiscard]] Ref<ParticleManager> GetParticleManager() const;
    [[nodiscard]] Ref<UIManager> GetUIManager() const;
    [[nodiscard]] Ref<TweenManager> GetTweenManager() const;
    [[nodiscard]] Ref<ShaderManager> GetShaderManager() const;

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
        return GetAssetManager()->GetAsset<T>(name);
      }
    }

    template <typename T> void LoadAsset(const fs::path &path)
    {
      std::string name = Base::Strings::ToLower(path.stem().string());
      _assets[name] = GetAssetManager()->LoadAsset<T>(path, false);
    }

    // Layer Management
    template <typename T> void AttachLayer(Base::Ref<RenderLayer> renderLayer)
    {
      _layerStack.AttachLayer<T>(renderLayer);
    }

    // Shared Data
    template <typename T> std::shared_ptr<T> GetSharedData()
    {
      return _state->sharedData.Get<T>();
    }

    // System Management
    void SuspendSystems();
    void UnsuspendSystems();
    void StartSystems();
    void StopSystems();

    const std::bitset<8> &GetPauseMask() const;
    const ShaderEffectChain &GetPostProcessingEffects() const;
  };
} // namespace Base
