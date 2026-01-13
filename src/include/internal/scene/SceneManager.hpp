#pragma once
#include "base/input/InputEvent.hpp"
#include "base/rendering/RenderingManager.hpp"
#include "base/scenes/Engine.hpp"
#include "base/scenes/SceneData.hpp"
#include "base/util/Ref.hpp"
#include "internal/input/InputListener.hpp"
#include <functional>
#include <memory>
#include <stack>
#include <typeindex>

namespace Base
{
  class Scene;
  class EntityManager;
  class SystemManager;
  class AssetManager;
  class TweenManager;
  class ParticleManager;
  class UIManager;
  class Renderer;
  class ShaderManager;
  class SceneManager : public InputListener
  {
    // Type Defs
    using QuitCallBack = std::function<void()>;
    using FactoryCallBack = std::function<std::shared_ptr<Scene>()>;

  private:
    QuitCallBack _quitCallBack = nullptr;
    std::unordered_map<std::type_index, FactoryCallBack> _factories;
    Ref<RenderingManager> _renderingManager;
    Ref<EntityManager> _entityManager;
    Ref<SystemManager> _systemManager;
    Ref<AssetManager> _assetManager;
    Ref<ParticleManager> _particleManager;
    Ref<UIManager> _uiManager;
    Ref<TweenManager> _tweenManager;
    Ref<ShaderManager> _shaderManager;

  private:
    std::stack<std::shared_ptr<Scene>> _scenes;
    std::type_index _startScene = typeid(nullptr);
    int64_t _currentSceneID = 0;
    EngineCtx _engine;

    void PushScene(std::type_index sceneID, const SceneData &sceneData = SceneData());
    void ReplaceScene(std::type_index sceneId, const SceneData &sceneData = SceneData());
    void PopScene();

  public:
    SceneManager(const EngineCtx &ctx);

    SceneManager() = default;
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory, bool startScene);
    void Update(float dt);
    void PostUpdate();
    void Render();

    // Input
    void OnInputEvent(std::shared_ptr<InputEvent> event) override;

    void SetQuitCallBack(QuitCallBack quitCallback);

    std::shared_ptr<const Scene> GetCurrentScene() const;
  };
} // namespace Base
