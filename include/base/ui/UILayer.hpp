#pragma once
#include "base/input/InputEvent.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UIPanel.hpp"
#include "base/util/Exception.hpp"
#include "raylib.h"
#include <memory>
#include <type_traits>

namespace Base
{
  class UILayer
  {
  private:
    std::shared_ptr<UIElement> _root = nullptr;
    std::shared_ptr<UIPanel> _layerBackPanel = nullptr;

    bool _isHidden = false;
    Vector2 _layerSize = {0, 0};
    Vector2 _layerPosition = {0, 0};

  public:
    UILayer() = default;
    UILayer(Vector2 layerSize, Vector2 layerPosition);
    template <typename T>
      requires(std::is_base_of_v<UIElement, T>)
    std::shared_ptr<T> SetRootElement()
    {

      if (_root)
      {
        THROW_BASE_RUNTIME_ERROR("Layer root element already set");
      }

      _root = std::make_shared<T>();
      return std::static_pointer_cast<T>(_root);
    }

    template <typename T>
      requires(std::is_base_of_v<UIElement, T>)
    std::shared_ptr<T> GetRootElement()
    {

      if (!_root)
      {
        THROW_BASE_RUNTIME_ERROR("Layer root element not set");
      }

      return std::static_pointer_cast<T>(_root);
    }

    std::shared_ptr<UIPanel> SetLayerBackPanel();
    void OnInputEvent(std::shared_ptr<InputEvent> &event);
    bool IsVisible() const;

    void Update(float dt);
    void Render();

    void Hide();
    void Show();
  };
} // namespace Base
