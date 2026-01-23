#pragma once
#include "base/scenes/Engine.hpp"
#include "base/scenes/SceneID.hpp"
#include "base/scenes/SceneLayerStack.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/scenes/SharedSceneDataStore.hpp"
#include "base/shaders/ShaderEffectChain.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
#include <bitset>
#include <memory>
#include <unordered_map>

namespace Base
{
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
    void _onInputEvent(std::shared_ptr<InputEvent> event);
    void _setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data = SceneData());

    void Init(SceneID id, const EngineCtx &ctx);
    void ResetSceneTransition();

    // Core
    void Render();
    void Update(float dt);

    // Template Methods
    void _exit();

    SceneID _sceneID;
    SceneTransition _sceneTransition = SceneTransition();

    Color _clearColor = {0, 0, 0, 255};
    SharedSceneDataStore<void> _sharedData;

    SceneLayerStack _layerStack;
    std::unordered_map<std::string, AssetHandle<void>> _assets;

    // Private Getters
    [[nodiscard]] const SceneTransition &GetSceneTransition() const;

    // Pause
    std::bitset<8> _pauseMask;

    // Engine
    EngineCtx _engine;

  protected:
    // Rendering
    void SetClearColor(Color color);
    Ref<RenderLayer> AddRenderLayer(Vector2 size, Color clearColor = {0, 0, 0, 0});

    ShaderEffectChain _postProcessingEffects;

    // Shader Effect Management
    template <typename T, typename... Args> auto AddPostProcessingEffect(Args &&...args) -> std::shared_ptr<T>
    {
      return _postProcessingEffects.AddEffect<T>(shared_from_this(), std::forward<Args>(args)...);
    }

    // Shared Data
    template <typename T> void InitSharedData()
    {
      auto data = SharedSceneDataStore<T>();
      data.Init();

      _sharedData = data;
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

    Color GetClearColor() const;
    [[nodiscard]] SceneID GetSceneID() const;

    [[nodiscard]] EngineCtx &Engine()
    {
      return _engine;
    }

    template <typename T = void> void SetSceneTransition(SceneRequest request, const SceneData &data = SceneData())
    {
      _setSceneTransition(typeid(T), request, data);
    }

    // Layer Management
    template <typename T> void AttachLayer(Base::Ref<RenderLayer> renderLayer)
    {
      _layerStack.AttachLayer<T>(renderLayer);
    }

    // Shared Data
    template <typename T> std::shared_ptr<T> GetSharedData()
    {
      return _sharedData.Get<T>();
    }

    const std::bitset<8> &GetPauseMask() const;
    const ShaderEffectChain &GetPostProcessingEffects() const;
  };
} // namespace Base
