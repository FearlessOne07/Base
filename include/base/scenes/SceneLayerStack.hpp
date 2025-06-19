#pragma once
#include "SceneLayer.hpp"
#include "base/renderer/RenderLayer.hpp"
#include "base/util/Exception.hpp"
#include <algorithm>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <vector>

namespace Base
{
  class SceneLayerStack
  {

  private:
    friend class Scene;
    std::vector<std::shared_ptr<SceneLayer>> _layers;
    std::vector<std::type_index> _layerIds;
    Scene *_owner = nullptr;
    int _currentLayer = 0;

    // Methods
    void DetachLayers();

  public:
    SceneLayerStack(Scene *owner);
    template <typename T>
    T *AttachLayer(RenderLayer *renderLayer)
    {
      if (std::is_base_of_v<SceneLayer, T>)
      {
        auto id = std::type_index(typeid(T));
        if (std::ranges::find(_layerIds, id) == _layerIds.end())
        {
          _layerIds.push_back(id);
          _layers.emplace_back(std::make_shared<T>());
          _layers.back()->_owner = _owner;
          _layers.back()->SetPauseMask(_currentLayer);
          _layers.back()->SetLayerIndex(_currentLayer);
          _layers.back()->_onAttach(renderLayer);
          _currentLayer++;
          return std::static_pointer_cast<T>(_layers.back()).get();
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Scene layer already attached");
        }
      }
      else
      {
        THROW_BASE_RUNTIME_ERROR("T must be a derivative of SceneLayer");
      }
    }

    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    void Update(float dt);
    void Render();
  };
} // namespace Base
