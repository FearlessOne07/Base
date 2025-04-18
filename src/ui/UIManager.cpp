#include "base/ui/UIManager.hpp"
#include "base/EventBus.hpp"
#include "base/ui/UILayer.hpp"
#include <stdexcept>

namespace Base
{

  void UIManager::Init()
  {
    EventBus *bus = EventBus::GetInstance();

    bus->SubscribeEvent<KeyEvent>([this](const std::shared_ptr<Event> &event) {
      this->OnKeyEvent(std::static_pointer_cast<KeyEvent>(event)); //
    });

    bus->SubscribeEvent<MouseButtonEvent>([this](const std::shared_ptr<Event> &event) {
      this->OnMouseButtonEvent(std::static_pointer_cast<MouseButtonEvent>(event)); //
    });
  }

  void UIManager::OnKeyEvent(const std::shared_ptr<KeyEvent> &event)
  {
    for (auto &[layerID, layer] : _uiLayers)
    {
      for (auto &[elementId, element] : layer._elements)
      {
        if (event->isHandled)
        {
          break;
        }
        element->OnKeyEvent(event);
      }
      if (event->isHandled)
      {
        break;
      }
    }
  }

  void UIManager::OnMouseButtonEvent(const std::shared_ptr<MouseButtonEvent> &event)
  {
    for (auto &[layerID, layer] : _uiLayers)
    {
      for (auto &[elementId, element] : layer._elements)
      {
        if (event->isHandled)
        {
          break;
        }
        element->OnMouseButtonEvent(event);
      }
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

    if (_uiLayers.find(layerId) == _uiLayers.end())
    {
      throw std::runtime_error("Specified UI layer does not exist");
    }
    else
    {
      for (auto &[id, element] : _uiLayers.at(layerId)._elements)
      {
        element->Render();
      }
    }
  }

  void UIManager::Clear()
  {
    _uiLayers.clear();
  }
} // namespace Base
