#include "base/ui/UILayer.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "base/ui/elements/UICanvas.hpp"
#include "base/util/Ref.hpp"
#include <memory>

namespace Base
{
  UILayer::UILayer(Vector2 layerSize, Vector2 layerPosition, ConstRef<SceneLayer> parentSceneLayer)
    : _layerSize(layerSize), _layerPosition(layerPosition), _parentSceneLayer(parentSceneLayer)
  {
  }

  void UILayer::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    if (_root)
    {
      if (!event->isHandled)
      {
        if (_root->IsVisible())
        {
          _root->OnInputEvent(event);
        }
      }
    }
  }

  void UILayer::Render()
  {
    if (_layerBackPanel && _layerBackPanel->IsVisible())
    {
      _layerBackPanel->Render(1);
    }

    if (_root && _root->IsVisible())
    {
      _root->Render(1);
    }
  }

  void UILayer::Update(float dt)
  {
    if (_layerBackPanel && _layerBackPanel->IsVisible())
    {
      _layerBackPanel->Arrange({{_layerPosition.x, _layerPosition.y}, {_layerSize.x, _layerSize.y}});
    }

    if (_root && _root->IsVisible())
    {
      Size rSize = _root->Measure();
      Vector2 rPos = _root->GetPosition();
      _root->Arrange({
        {
          _layerPosition.x + rPos.x,
          _layerPosition.y + rPos.y,
        },
        {_layerSize.x, _layerSize.y},
      });

      if (_root->IsVisible())
      {
        _root->Update(dt, {.MousePosition = _parentSceneLayer->GetLayerMousePosition()});
      }
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
    _layerBackPanel->SetSize({_layerSize.x, _layerSize.y});
    return _layerBackPanel;
  }
} // namespace Base
