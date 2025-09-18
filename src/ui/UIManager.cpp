#include "base/ui/UIManager.hpp"
#include "base/util/Exception.hpp"
#include "base/util/Strings.hpp"
#include "raylib.h"

namespace Base
{
  void UIManager::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    for (auto &[id, layer] : _layers)
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

  UILayer *UIManager::AddLayer(const std::string &layerID, Vector2 layerSize, Vector2 layerPosition)
  {
    std::string lowerID = Base::Strings::ToLower(layerID);
    if (_layers.contains(lowerID))
    {
      THROW_BASE_RUNTIME_ERROR("Layer " + layerID + " already exists");
    }

    _layers[lowerID] = UILayer(layerSize, layerPosition);
    return &_layers.at(lowerID);
  }

  UILayer *UIManager::GetLayer(const std::string &layerID)
  {
    std::string lowerID = Base::Strings::ToLower(layerID);
    if (!_layers.contains(lowerID))
    {
      THROW_BASE_RUNTIME_ERROR("Layer " + layerID + " doesnot exist");
    }

    return &_layers.at(lowerID);
  }

  void UIManager::RemoveLayer(const std::string &layerID)
  {
    std::string lowerID = Base::Strings::ToLower(layerID);
    if (!_layers.contains(lowerID))
    {
      THROW_BASE_RUNTIME_ERROR("Layer " + layerID + " does not exist");
    }
    _layers.erase(lowerID);
  }

  void UIManager::RenderLayer(const std::string &layerID)
  {
    std::string lowerID = Base::Strings::ToLower(layerID);
    if (!_layers.contains(lowerID))
    {
      THROW_BASE_RUNTIME_ERROR("Layer " + layerID + " does not exist");
    }
    _layers.at(lowerID).Render();
  }

  void UIManager::Update(float dt)
  {
    for (auto &[id, layer] : _layers)
    {
      layer.Update(dt);
    }
  }
} // namespace Base
