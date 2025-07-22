#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/renderer/Renderer.hpp"
#include "base/scenes/SceneLayerStack.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/scenes/SharedSceneDataStore.hpp"
#include "base/shaders/ShaderManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
#include "base/util/Strings.hpp"
#include "raylib.h"
#include <bitset>
#include <filesystem>
#include <memory>

namespace Base
{

  namespace fs = std::filesystem;
  class EntityManager;
  class SystemManager;
  class AssetManager;
  class Scene
  {

  private:
    friend class SceneManager;
    void SetEntityManager(EntityManager *);
    void SetParticleManager(ParticleManager *);
    void SetAssetManager(AssetManager *);
    void SetSystemManager(SystemManager *);
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
      UIManager *uiManager = nullptr;
      TweenManager *tweenManager = nullptr;
      ShaderManager *shaderManager = nullptr;
      Color clearColor = BLACK;
      SharedSceneDataStore<void> sharedData;
    };

    std::unique_ptr<SceneState> _state;
    SceneLayerStack _layerStack;
    std::unordered_map<std::string, AssetHandle<void>> _assets;

    // Private Getters
    [[nodiscard]] Renderer *GetRenderer() const;
    [[nodiscard]] AssetManager *GetAssetManager() const;
    [[nodiscard]] SceneTransition GetSceneTransition() const;

    // Pause
    std::bitset<8> _pauseMask;

    // Scene Store

  protected:
    // Rendering
    void SetClearColor(Color color);
    RenderLayer *AddRenderLayer(Vector2 size, Color clearColor = BLANK);

    // Shared Data
    template <typename T> void InitSharedData()
    {
      auto data = SharedSceneDataStore<T>();
      data.Init();

      _state->sharedData = data;
    }

  public:
    Scene();
    virtual ~Scene() = default;
    virtual void Enter(SceneData sceneData = SceneData()) = 0;
    virtual void Exit() = 0;
    virtual void Resume();
    virtual void Suspend();
    virtual void OnInputEvent(std::shared_ptr<InputEvent> event);
    void PauseMenu(int layerIndex);
    void UnPauseMenu(int layerIndex);
    bool IsMenuPaused(int layerIndex);

    void _OnInputEvent(std::shared_ptr<InputEvent> event);

    Color GetClearColor() const;
    [[nodiscard]] EntityManager *GetEntityManager() const;
    [[nodiscard]] SystemManager *GetSystemManager() const;
    [[nodiscard]] ParticleManager *GetParticleManager() const;
    [[nodiscard]] UIManager *GetUIManager() const;
    [[nodiscard]] TweenManager *GetTweenManager() const;
    [[nodiscard]] ShaderManager *GetShaderManager() const;

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
    template <typename T> void AttachLayer(RenderLayer *renderLayer)
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
  };
} // namespace Base
