#include "base/ui/UIManager.hpp"
#include "base/ui/UILayer.hpp"
#include <stdexcept>

namespace Base
{
  void UIManager::AddElement(                                                                           //
    const std::string &layerId, const std::string &elementId, const std::shared_ptr<UIElement> &element //
  )
  {
    if (_uiLayers.find(layerId) == _uiLayers.end())
    {
      throw std::runtime_error("Specified UI layer does not exist");
    }
    else
    {
      _uiLayers.at(layerId).Add(elementId, element);
    }
  }

  void UIManager::AddLayer(const std::string &layerId)
  {
    if (_uiLayers.find(layerId) == _uiLayers.end())
    {
      _uiLayers[layerId] = UILayer();
    }
  }

  void UIManager::RemoveElement(                             //
    const std::string &layerId, const std::string &elementId //
  )
  {
    if (_uiLayers.find(layerId) == _uiLayers.end())
    {
      throw std::runtime_error("Specified UI layer does not exist");
    }
    else
    {
      _uiLayers.at(layerId).Remove(elementId);
    }
  }

  void UIManager::RemoveLayer(const std::string &layerId)
  {
    if (_uiLayers.find(layerId) == _uiLayers.end())
    {
      throw std::runtime_error("Specified UI layer does not exist");
    }
    else
    {
      auto it = _uiLayers.find(layerId);
      _uiLayers.erase(it);
    }
  }

  std::shared_ptr<UIElement> &UIManager::GetElement(const std::string &layerId, const std::string &elementId)
  {
    if (_uiLayers.find(layerId) == _uiLayers.end())
    {
      throw std::runtime_error("Specified UI layer does not exist");
    }
    else
    {
      return _uiLayers.at(layerId).Get(elementId);
    }
  }

  bool UIManager::HasElement(const std::string &layerID, const std::string &elementId) const
  {
    if (_uiLayers.find(layerID) == _uiLayers.end())
    {
      throw std::runtime_error("Specified UI Layer does not exist");
    }
    else
    {
      return _uiLayers.at(layerID).HasElement(elementId);
    }
  }

  void UIManager::RenderLayer(const std::string &layerId)
  {
    for (auto &[id, element] : _uiLayers.at(layerId)._elements)
    {
      element->Render();
    }
  }

  void UIManager::Update()
  {
    for (auto &[layerId, layer] : _uiLayers)
    {
      for (auto &[elementId, element] : _uiLayers.at(layerId)._elements)
      {
        element->Update();
      }
    }
  }

  void UIManager::Clear()
  {
    _uiLayers.clear();
  }
} // namespace Base
