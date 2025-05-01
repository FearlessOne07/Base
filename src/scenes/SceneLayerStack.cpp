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
      layer->OnInputEvent(event);
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
      layer->Update(dt);
    }
  }

  void SceneLayerStack::Render()
  {
    for (auto &_layer : std::ranges::reverse_view(_layers))
    {
      _layer->Render();
    }
  }
} // namespace Base
