#include "base/ui/elements/UIContainer.hpp"
#include <ranges>
namespace Base
{
  void UIContainer::Update(float dt)
  {
    switch (_layout)
    {
    case Layout::VERTICAL:
      LayoutVertical();
      break;
    case Layout::HORIZONTAL:
      LayoutHorizontal();
      break;
    }

    for (auto &element : _childElements)
    {
      element->Update(dt);
    }
  }

  void UIContainer::LayoutVertical()
  {
    float currentOffset = 0;
    Vector2 newContainerSize = {0, 0};
    for (auto &element : _childElements)
    {
      if (currentOffset == 0)
      {
        currentOffset += _padding.y;
      }
      else
      {
        currentOffset += _gapSize;
      }
      newContainerSize.x = std::max(newContainerSize.x, element->GetSize().x);
      currentOffset += element->GetSize().y;
    }
    currentOffset += _padding.x;
    newContainerSize.x += _padding.x * 2;
    newContainerSize.y = currentOffset;
    _size = newContainerSize;
    UpdatePosition();

    // Calculate Layout
    currentOffset = 0;
    for (auto &element : _childElements)
    {
      const UILayoutSettings &layoutSettings = element->GetLayoutSettings();
      Vector2 nextPosition = {0, 0};

      switch (layoutSettings.hAlignment)
      {
      case UIHAlignment::LEFT:
        nextPosition.x = _position.x + _padding.x;
        break;
      case UIHAlignment::CENTER:
        nextPosition.x = _position.x + _size.x / 2.f;
        nextPosition.x -= element->GetSize().x / 2;
        break;
      case UIHAlignment::RIGHT:
        nextPosition.x = (_position.x + _size.x) - _padding.x - element->GetSize().x;
        break;
      }

      if (currentOffset == 0)
      {
        currentOffset += _padding.y;
        nextPosition.y = _position.y + currentOffset;
      }
      else
      {
        currentOffset += _gapSize;
        nextPosition.y = _position.y + currentOffset;
      }
      currentOffset += element->GetSize().y;
      element->SetPosition(nextPosition);
    }
  }

  void UIContainer::LayoutHorizontal()
  {
  }

  void UIContainer::UpdatePosition()
  {
    switch (_anchorPoint)
    {
    case TOP_LEFT:
      break;
    case TOP_CENTER:
      _position.x = _setPosition.x - (_size.x / 2);
      break;
    case TOP_RIGHT:
      _position.x = _setPosition.x - _size.x;
      break;
    case RIGHT_CENTER:
      _position.x = _setPosition.x - _size.x;
      _position.y = _setPosition.y - (_size.y / 2);
      break;
    case BOTTOM_RIGHT:
      _position.x = _setPosition.x - _size.x;
      _position.y = _setPosition.y - _size.y;
      break;
    case BOTTOM_CENTER:
      _position.y = _setPosition.y - (_size.y / 2);
      break;
    case BOTTOM_LEFT:
      _position.y = _setPosition.y - _size.y;
      break;
    case LEFT_CENTER:
      _position.y = _setPosition.y - (_size.y / 2);
      break;
    case CENTER:
      _position.x = _setPosition.x - (_size.x / 2);
      _position.y = _setPosition.y - (_size.y / 2);
      break;
    }
  }

  void UIContainer::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    for (auto &element : _childElements)
    {
      if (element->IsVisible())
      {
        element->OnInputEvent(event);
      }
    }
  }

  void UIContainer::SetAnchorPoint(AnchorPoint anchorPoint)
  {
    _anchorPoint = anchorPoint;
  }

  void UIContainer::Render()
  {
    auto elements = std::ranges::reverse_view(_childElements);
    for (auto &element : elements)
    {
      element->Render();
    }
  }

  void UIContainer::Hide()
  {
    _isHidden = true;
    if (_onHide)
    {
      _onHide();
    }

    for (auto &element : _childElements)
    {
      element->Hide();
    }
  }

  void UIContainer::Show()
  {
    _isHidden = false;
    if (_onShow)
    {
      _onShow();
    }

    for (auto &element : _childElements)
    {
      element->Show();
    }
  }

  void UIContainer::SetGapSize(float gapSize)
  {
    _gapSize = gapSize;
  }

  void UIContainer::SetPadding(Vector2 padding)
  {
    _padding = padding;
  }

  void UIContainer::SetLayout(Layout layout)
  {
    _layout = layout;
  }
} // namespace Base
