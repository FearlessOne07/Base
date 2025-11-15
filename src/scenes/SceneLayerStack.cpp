#include <ranges>

#include "base/scenes/SceneLayerStack.hpp"

namespace Base
{
  SceneLayerStack::SceneLayerStack(Scene *owner) : _owner(owner)
  {
  }

  void SceneLayerStack::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    for (auto &layer : _layers)
    {
      if (!layer->IsPaused())
      {
        layer->OnInputEvent(event);
      }
      if (event->isHandled)
      {
        break;
      }
    }
  }

  void SceneLayerStack::Update(float dt)
  {
    for (auto &layer : _layers)
    {
      if (!layer->IsPaused())
      {
        layer->Update(dt);
      }
    }
  }

  void SceneLayerStack::Render()
  {
    for (auto &_layer : std::ranges::reverse_view(_layers))
    {
      _layer->Render();
    }
  }

  void SceneLayerStack::DetachLayers()
  {
    for (auto &_layer : _layers)
    {
      _layer->_onDetach();
    }
    _layers.clear();
    _layerIds.clear();
  }
} // namespace Base
