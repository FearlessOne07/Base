#include "base/scenes/Scene.hpp"
#include "base/rendering/RenderingManager.hpp"
#include "base/scenes/SceneData.hpp"
#include "base/scenes/signals/SceneLayerPausedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/systems/SystemManager.hpp"
#include "internal/rendering/Renderer.hpp"
#include <memory>

namespace Base
{

  SceneID::operator bool() const
  {
    return _id >= 0;
  }

  SceneID::operator int64_t() const
  {
    return _id;
  }

  bool SceneID::operator==(const SceneID &other) const
  {
    return _id == other._id;
  }

  SceneID::SceneID(int64_t id)
  {
    _id = id;
  }

  SceneID::SceneID()
  {
    _id = -1;
  }

  void Scene::_setSceneTransition(std::type_index sceneID, SceneRequest request, const SceneData &data)
  {
    _sceneTransition = {
      .request = request,
      .sceneID = sceneID,
      .data = data,
    };
  }

  SceneID Scene::GetSceneID() const
  {
    return _sceneID;
  }

  const SceneTransition &Scene::GetSceneTransition() const
  {
    return _sceneTransition;
  }
  void Scene::ResetSceneTransition()
  {
    _sceneTransition = {.request = SceneRequest::None, .sceneID = typeid(-1)};
  }

  void Scene::Init(SceneID id, const EngineCtx &ctx)
  {
    _sceneID = id;
    _engine = ctx;
    _layerStack = SceneLayerStack(shared_from_this());
  }

  void Scene::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
  }

  void Scene::_onInputEvent(std::shared_ptr<InputEvent> event)
  {
    OnInputEvent(event);
    _layerStack.OnInputEvent(event);
  }

  void Scene::Update(float dt)
  {
    _layerStack.Update(dt);
  }

  void Scene::Render()
  {
    Renderer::Clear(_clearColor);
    _layerStack.Render();
  }

  void Scene::Resume()
  {
  }

  void Scene::Suspend()
  {
  }

  // Temmplate Methods
  void Scene::_exit()
  {
    _layerStack.DetachLayers();
    Exit();
  }

  Ref<RenderLayer> Scene::AddRenderLayer(Vector2 size, Color clearColor)
  {
    return _engine.Rendering->InitLayer(shared_from_this(), {0, 0}, size, clearColor);
  }

  void Scene::PauseLayer(int layerIndex)
  {
    auto bus = SignalBus::GetInstance();
    bus->BroadCastSignal(std::make_shared<SceneLayerPausedSignal>());
    _pauseMask.set(layerIndex);
  }

  void Scene::UnPauseLayer(int layerIndex)
  {
    _pauseMask.reset(layerIndex);
  }

  bool Scene::IsLayerPaused(int layerIndex)
  {
    return _pauseMask.test(layerIndex);
  }

  const std::bitset<8> &Scene::GetPauseMask() const
  {
    return _pauseMask;
  }

  const ShaderEffectChain &Scene::GetPostProcessingEffects() const
  {
    return _postProcessingEffects;
  }

  Color Scene::GetClearColor() const
  {
    return _clearColor;
  }

  void Scene::SetClearColor(Color color)
  {
    _clearColor = color;
  }

  Vector2 Rectangle::GetPosition() const
  {
    return _position;
  }
} // namespace Base
