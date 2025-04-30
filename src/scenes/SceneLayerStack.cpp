#include <ranges>

#include "base/scenes/SceneLayerStack.hpp"

namespace Base
{

  std::map<std::type_index, std::shared_ptr<SceneLayer>>::iterator SceneLayerStack::begin()
  {
    return _layers.begin();
  }

  std::map<std::type_index, std::shared_ptr<SceneLayer>>::iterator SceneLayerStack::end()
  {
    return _layers.end();
  }

  void SceneLayerStack::OnInputEvent(std::shared_ptr<InputEvent> event)
  {
    for (auto &[id, layer] : _layers)
    {
      layer->OnInputEvent(event);
    }
  }

  void SceneLayerStack::Update(float dt)
  {
    for (auto &[id, layer] : _layers)
    {
      layer->Update(dt);
    }
  }

  void SceneLayerStack::Render()
  {
    for (auto &_layer : std::ranges::reverse_view(_layers))
    {
      _layer.second->Render();
    }
  }
} // namespace Base
