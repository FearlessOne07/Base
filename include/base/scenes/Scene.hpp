#pragma once
#include "base/game/GameContext.hpp"
#include "base/scenes/SceneLayerStack.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/scenes/SharedSceneDataStore.hpp"
#include "base/shaders/ShaderEffectChain.hpp"
#include "base/shaders/ShaderManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
#include "raylib.h"
#include <bitset>
#include <memory>

namespace Base
{
  class Scene : public std::enable_shared_from_this<Scene>
  {

  private:
    friend class SceneManager;
    void Init();
    void SetGameCtx(const GameContext &ctx);
    void SetSceneID(SceneID);
    void _setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data = SceneData());
    void ResetSceneTransition();

    // Core
    void Render();
    void Update(float dt);

    // Template Methods
    void _exit();

    // Scene State
    SceneID _sceneID;
    GameContext _ctx;
    SceneTransition _sceneTransition = SceneTransition();
    Color _clearColor = BLACK;
    SharedSceneDataStore<void> _sharedData;

    SceneLayerStack _layerStack;

    // Pause
    std::bitset<8> _pauseMask;

    // Scene Store

  protected:
    // Rendering
    void SetClearColor(Color color);
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

    void _OnInputEvent(std::shared_ptr<InputEvent> event);

    Color GetClearColor() const;
    [[nodiscard]] const SceneTransition &GetSceneTransition() const;
    [[nodiscard]] SceneID GetSceneID() const;
    [[nodiscard]] GameContext GameCtx() const;

    template <typename T = void> void SetSceneTransition(SceneRequest request, const SceneData &data = SceneData())
    {
      _setSceneTransition(typeid(T), request, data);
    }

    // Layer Management
    template <typename T> void AttachLayer(Base::Ref<RenderLayer> renderLayer)
    {
      _layerStack.AttachLayer<T>(renderLayer, _ctx);
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
