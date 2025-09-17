#include "base/ui/UILayer.hpp"
#include "raylib.h"
#include <memory>

namespace Base
{
  UILayer::UILayer(Vector2 layerSize) : _layerSize(layerSize)
  {
  }

  void UILayer::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    if (_root)
    {
      if (!event->isHandled)
      {
        _root->OnInputEvent(event);
      }
    }
  }

  void UILayer::Render()
  {
    if (_layerBackPanel)
    {
      _layerBackPanel->Render(1);
    }
    if (_root)
    {
      _root->Render(1);
    }
  }

  void UILayer::Update(float dt)
  {
    if (_layerBackPanel)
    {
      _layerBackPanel->Arrange({0, 0, _layerSize.x, _layerSize.y});
    }

    if (_root)
    {
      _root->Measure();
      _root->Arrange({0, 0, _layerSize.x, _layerSize.y});
      _root->Update(dt);
    }
  }

  void UILayer::Hide()
  {
    if (_layerBackPanel)
    {
      _layerBackPanel->Hide();
    }
    if (_root)
    {
      _root->Hide();
    }
    _isHidden = true;
  }

  void UILayer::Show()
  {

    if (_layerBackPanel)
    {
      _layerBackPanel->Show();
    }

    if (_root)
    {
      _root->Show();
    }
    _isHidden = false;
  }

  bool UILayer::IsVisible() const
  {
    return !_isHidden;
  }

  std::shared_ptr<UIPanel> UILayer::SetLayerBackPanel()
  {
    _layerBackPanel = std::make_shared<UIPanel>();
    return _layerBackPanel;
  }
} // namespace Base
