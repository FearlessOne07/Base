#pragma once
#include "SceneLayer.hpp"
#include "base/util/Exception.hpp"
#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <vector>

namespace Base
{
  class SceneLayerStack
  {

  private:
    std::vector<std::shared_ptr<SceneLayer>> _layers;
    std::vector<std::type_index> _layerIds;
    Scene *_owner = nullptr;

  public:
    SceneLayerStack(Scene *owner);
    template <typename T> void AttachLayer()
    {
      if (std::is_base_of_v<SceneLayer, T>)
      {
        auto id = std::type_index(typeid(T));
        if (std::ranges::find(_layerIds, id) == _layerIds.end())
        {
          _layerIds.push_back(id);
          _layers.emplace_back(std::make_shared<T>());
          _layers.back()->_owner = _owner;
          _layers.back()->OnAttach();
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

    template <typename T> void DetachLayer()
    {
      if (std::is_base_of_v<SceneLayer, T>)
      {
        auto id = std::type_index(typeid(T));
        if (auto it = std::ranges::find(_layerIds, id); it == _layerIds.end())
        {
          int index = static_cast<int>(std::distance(_layerIds.begin(), it));
          _layerIds.erase(it);
          _layers.erase(_layers.begin() + index);
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Scene layer does not exist");
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
