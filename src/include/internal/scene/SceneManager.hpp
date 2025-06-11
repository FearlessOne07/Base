#pragma once
#include "base/camera/CameraManager.hpp"
#include "base/input/InputEvent.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/renderer/Renderer.hpp"
#include "base/scenes/SceneData.hpp"
#include "base/shaders/ShaderManager.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIManager.hpp"
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
  class SceneManager : public InputListener
  {
    // Type Defs
    using QuitCallBack = std::function<void()>;
    using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

  private:
    QuitCallBack _quitCallBack = nullptr;
    std::unordered_map<std::type_index, FactoryCallBack> _factories;
    Renderer *_renderer = nullptr;
    EntityManager *_entityManager = nullptr;
    SystemManager *_systemManager = nullptr;
    AssetManager *_assetManager = nullptr;
    ParticleManager *_particleManager = nullptr;
    CameraManager *_cameraManager = nullptr;
    UIManager *_uiManager = nullptr;
    TweenManager *_tweenManager = nullptr;
    ShaderManager *_shaderManager = nullptr;

  private:
    std::stack<std::shared_ptr<Scene>> _scenes;
    std::type_index _startScene = typeid(nullptr);

    void PushScene(std::type_index sceneID, const SceneData &sceneData = SceneData());
    void ReplaceScene(std::type_index sceneId, const SceneData &sceneData = SceneData());
    void PopScene();

  public:
    SceneManager(                                                                                                 //
      Renderer *renderer, EntityManager *entityManager, SystemManager *systemManager, AssetManager *assetManager, //
      ParticleManager *particleManager, CameraManager *cameraManager, UIManager *uiManager, TweenManager *tweenManager,
      ShaderManager *shaderManager //
    );
    SceneManager() = default;
    void RegisterScene(std::type_index sceneID, FactoryCallBack factory, bool startScene);
    void Update(float dt);
    void Render();

    // Input
    void OnInputEvent(std::shared_ptr<InputEvent> event) override;

    void SetQuitCallBack(QuitCallBack quitCallback);
  };
} // namespace Base
