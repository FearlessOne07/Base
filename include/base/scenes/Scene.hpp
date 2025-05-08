#pragma once
#include "base/camera/CameraManager.hpp"
#include "base/particles/ParticleManager.hpp"
#include "base/scenes/SceneLayerStack.hpp"
#include "base/scenes/SceneTransition.hpp"
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
    void ResetSceneTransition();
    void _setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data = SceneData());
    void Render();
    void Update(float dt);

    struct SceneState
    {
      SceneTransition sceneTransition = SceneTransition();
      EntityManager *entityManager = nullptr;
      ParticleManager *particleManager = nullptr;
      AssetManager *assetManager = nullptr;
      SystemManager *systemManager = nullptr;
      CameraManager *cameraManager = nullptr;
    };

    std::unique_ptr<SceneState> _state;
    SceneLayerStack _layerStack;

  public:
    Scene();
    virtual ~Scene() = default;
    virtual void Enter(SceneData sceneData = SceneData()) = 0;
    virtual void Exit() = 0;
    virtual void OnInputEvent(std::shared_ptr<InputEvent> event);

    [[nodiscard]] EntityManager *GetEntityManager() const;
    [[nodiscard]] SystemManager *GetSystemManager() const;
    [[nodiscard]] ParticleManager *GetParticleManager() const;
    [[nodiscard]] AssetManager *GetAssetManager() const;
    [[nodiscard]] CameraManager *GetCameraManager() const;
    [[nodiscard]] SceneLayerStack &GetLayerStack();

    template <typename T = void> void SetSceneTransition(SceneRequest request, const SceneData &data = SceneData())
    {
      _setSceneTransition(typeid(T), request, data);
    }
  };
} // namespace Base
