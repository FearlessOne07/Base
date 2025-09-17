#include "base/ui/UILayer.hpp"

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
    if (!_isHidden && _root)
    {
      _root->Render();
    }
  }

  void UILayer::Update(float dt)
  {
    if (_root)
    {
      _root->Measure();
      _root->Arrange({0, 0, _layerSize.x, _layerSize.y});
      _root->Update(dt);
    }
  }

  void UILayer::Hide()
  {
    if (_root)
    {
      _root->Hide();
    }
    _isHidden = true;
  }

  void UILayer::Show()
  {
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
} // namespace Base
