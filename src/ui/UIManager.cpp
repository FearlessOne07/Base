#include "base/ui/UIManager.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/ui/UILayer.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Ref.hpp"
#include "base/util/Strings.hpp"
#include <utility>

namespace Base
{
  void UIManager::Init()
  {
    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> sig) {
      auto scenePushed = std::static_pointer_cast<ScenePushedSignal>(sig);
      UpdateCurrentScene(scenePushed->Scene);
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> sig) {
      auto sceneResumed = std::static_pointer_cast<SceneResumedSignal>(sig);
      UpdateCurrentScene(sceneResumed->Scene);
    });

    bus->SubscribeSignal<ScenePoppedSignal>([this](std::shared_ptr<Signal> signal) {
      auto scenePopped = std::static_pointer_cast<ScenePoppedSignal>(signal);
      UnloadSceneUI(scenePopped->Scene);
    });
  }

  void UIManager::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    if (_currentScene)
    {
      for (auto &[id, layer] : _layers.at(_currentScene))
      {
        if (layer.IsVisible())
        {
          layer.OnInputEvent(event);
        }

        if (event->isHandled)
        {
          break;
        }
      }
    }
  }

  Ref<UILayer> UIManager::AddLayer( //
    const std::string &layerID, Vector2 layerSize, Vector2 layerPosition,
    ConstRef<SceneLayer> parentLayer //
  )
  {
    std::string lowerID = Base::Strings::ToLower(layerID);
    if (_currentScene)
    {
      if (_layers.at(_currentScene).contains(lowerID))
      {
        THROW_BASE_RUNTIME_ERROR("Layer " + layerID + " already exist");
      }
      else
      {
        _layers.at(_currentScene)
          .emplace( //
            std::piecewise_construct, std::forward_as_tuple(layerID),
            std::forward_as_tuple(layerSize, layerPosition, parentLayer) //
          );
        return _layers.at(_currentScene)[layerID];
      }
    }
    return Ref<UILayer>();
  }

  void UIManager::RenderLayer(Ref<UILayer> layer)
  {
    if (_currentScene)
    {
      layer->Render();
    }
  }

  void UIManager::Update(float dt)
  {
    if (_currentScene)
    {
      for (auto &[id, layer] : _layers[_currentScene])
      {
        layer.Update(dt);
      }
    }
  }

  void UIManager::UnloadSceneUI(SceneID scene)
  {
    _layers.erase(_currentScene);
    _currentScene = SceneID();
  }

  void UIManager::UpdateCurrentScene(SceneID scene)
  {
    _currentScene = scene;
    _layers[_currentScene];
  }
} // namespace Base
