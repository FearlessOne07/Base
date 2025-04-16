#pragma once

#include "base/input/Events/KeyEvent.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/UILayer.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace Base
{
  class UIManager
  {
  private:
    std::unordered_map<std::string, UILayer> _uiLayers = {};

  public:
    void Init();

    void OnKeyEvent(const std::shared_ptr<KeyEvent> &event);
    void OnMouseButtonEvent(const std::shared_ptr<MouseButtonEvent> &event);

  public:
    void AddElement(                                                                                      //
      const std::string &layerId, const std::string &elementID, const std::shared_ptr<UIElement> &element //
    );
    void AddLayer(const std::string &layerId);

    void RemoveElement(const std::string &layerId, const std::string &elementID);
    void RemoveLayer(const std::string &layerId);

    bool HasElement(const std::string &layerID, const std::string &elementId) const;

    std::shared_ptr<UIElement> &GetElement(const std::string &layerId, const std::string &elementID);

    void RenderLayer(const std::string &layerId);
    void Update();
    void Clear();
  };
} // namespace Base
