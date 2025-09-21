#include "base/ui/UIManager.hpp"
#include "base/scenes/signals/ScenePoppedSignal.hpp"
#include "base/scenes/signals/ScenePushedSignal.hpp"
#include "base/scenes/signals/SceneResumedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include "raylib.h"

namespace Base
{
  void UIManager::Init()
  {
    auto bus = SignalBus::GetInstance();
    bus->SubscribeSignal<ScenePushedSignal>([this](std::shared_ptr<Signal> sig) {
      auto scenePushed = std::static_pointer_cast<ScenePushedSignal>(sig);
      UpdateCurrentScene(scenePushed->scene);
    });

    bus->SubscribeSignal<SceneResumedSignal>([this](std::shared_ptr<Signal> sig) {
      auto sceneResumed = std::static_pointer_cast<SceneResumedSignal>(sig);
      UpdateCurrentScene(sceneResumed->scene);
    });

    bus->SubscribeSignal<ScenePoppedSignal>([this](std::shared_ptr<Signal> signal) {
      auto scenePopped = std::static_pointer_cast<ScenePoppedSignal>(signal);
      UnloadSceneUI(scenePopped->scene);
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

  UILayer *UIManager::AddLayer(const std::string &layerID, Vector2 layerSize, Vector2 layerPosition)
  {
    std::string lowerID = Base::Strings::ToLower(layerID);
    if (_currentScene)
    {
      if (_layers[_currentScene].contains(lowerID))
      {
        THROW_BASE_RUNTIME_ERROR("Layer " + layerID + " already exist");
      }
      else
      {
        _layers[_currentScene][layerID] = UILayer(layerSize, layerPosition);
        return &_layers[_currentScene][layerID];
      }
    }
    return nullptr;
  }

  void UIManager::RenderLayer(const std::string &layerID)
  {
    if (_currentScene)
    {
      std::string lowerID = Base::Strings::ToLower(layerID);
      if (!_layers[_currentScene].contains(lowerID))
      {
        THROW_BASE_RUNTIME_ERROR("Layer " + layerID + " does not exist");
      }
      _layers[_currentScene].at(lowerID).Render();
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

  void UIManager::UnloadSceneUI(const Scene *scene)
  {
    _layers.erase(_currentScene);
    _currentScene = nullptr;
  }

  void UIManager::UpdateCurrentScene(const Scene *scene)
  {
    _currentScene = scene;
  }
} // namespace Base
