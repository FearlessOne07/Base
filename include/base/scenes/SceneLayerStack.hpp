#pragma once
#include "SceneLayer.hpp"
#include "base/util/Exception.hpp"
#include <map>
#include <memory>
#include <type_traits>
#include <typeindex>

namespace Base
{
  class SceneLayerStack
  {

  private:
    std::map<std::type_index, std::shared_ptr<SceneLayer>> _layers;
    Scene *_owner = nullptr;

  public:
    SceneLayerStack(Scene *owner);
    template <typename T> void AttachLayer()
    {
      if (std::is_base_of_v<SceneLayer, T>)
      {
        auto id = std::type_index(typeid(T));
        if (_layers.find(id) == _layers.end())
        {
          _layers[id] = std::make_shared<T>();
          _layers.at(id)->_owner = _owner;
        }
        else
        {
          THROW_BASE_RUNTIME_ERROR("Scene layer already exists");
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
        if (_layers.find(id) != _layers.end())
        {
          _layers.erase(id);
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

    void OnInputEvent(std::shared_ptr<InputEvent>);
    void Update(float dt);
    void Render();
  };
} // namespace Base
