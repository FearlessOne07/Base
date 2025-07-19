#include "base/ui/elements/UIContainer.hpp"
#include "raylib.h"
#include <memory>
#include <ranges>

namespace Base
{
  void UIContainer::Update(float dt)
  {
    UpdatePosition();
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
      element->_update(dt);
    }
  }

  void UIContainer::LayoutVertical()
  {
    // AUTO GAP CALCULATION
    if (_gapMode == GapMode::AUTO && !_childElements.empty())
    {
      float totalElementHeight = 0;
      for (const auto &element : _childElements)
      {
        totalElementHeight += element->GetBaseSize().y;
      }

      int gapCount = static_cast<int>(_childElements.size()) - 1;
      float availableHeight = _baseSize.y - _padding.y * 2 - totalElementHeight;
      _gapSize = gapCount > 0 ? std::max(0.0f, availableHeight / gapCount) : 0.0f;
    }

    float currentOffset = 0;
    if (_elementSizeMode == ElementSizeMode::FIT)
    {
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
        newContainerSize.x = std::max(newContainerSize.x, element->GetBaseSize().x);
        currentOffset += element->GetBaseSize().y;
      }
      currentOffset += _padding.x;
      newContainerSize.x += _padding.x * 2;
      newContainerSize.y = currentOffset;
      _baseSize = newContainerSize;
      _currentSize = newContainerSize;
    }
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
        nextPosition.x = GetPosition().x + _padding.x;
        break;
      case UIHAlignment::CENTER:
        nextPosition.x = GetPosition().x + _baseSize.x / 2.f;
        nextPosition.x -= element->GetSize().x / 2;
        break;
      case UIHAlignment::RIGHT:
        nextPosition.x = (GetPosition().x + _baseSize.x) - _padding.x - element->GetSize().x;
        break;
      }

      if (currentOffset == 0)
      {
        currentOffset += _padding.y;
        nextPosition.y = GetPosition().y + currentOffset;
      }
      else
      {
        currentOffset += _gapSize;
        nextPosition.y = GetPosition().y + currentOffset;
      }
      currentOffset += element->GetBaseSize().y;

      if (std::dynamic_pointer_cast<UIContainer>(element))
      {
        element->SetPosition(nextPosition);
      }
      else
      {
        element->_layoutPosition = nextPosition;
      }
    }
  }

  void UIContainer::LayoutHorizontal()
  {
    // AUTO GAP CALCULATION
    if (_gapMode == GapMode::AUTO && !_childElements.empty())
    {
      float totalElementWidth = 0;
      for (const auto &element : _childElements)
      {
        totalElementWidth += element->GetBaseSize().x;
      }

      int gapCount = static_cast<int>(_childElements.size()) - 1;
      float availableWidth = _baseSize.x - _padding.x * 2 - totalElementWidth;
      _gapSize = gapCount > 0 ? std::max(0.0f, availableWidth / gapCount) : 0.0f;
    }

    float currentOffset = 0;
    if (_elementSizeMode == ElementSizeMode::FIT)
    {
      Vector2 newContainerSize = {0, 0};
      for (auto &element : _childElements)
      {
        if (currentOffset == 0)
        {
          currentOffset += _padding.x;
        }
        else
        {
          currentOffset += _gapSize;
        }
        newContainerSize.y = std::max(newContainerSize.y, element->GetBaseSize().y);
        currentOffset += element->GetBaseSize().x;
      }
      currentOffset += _padding.y;
      newContainerSize.y += _padding.y * 2;
      newContainerSize.x = currentOffset;
      _baseSize = newContainerSize;
      _currentSize = newContainerSize;
    }
    UpdatePosition();

    // Calculate Layout
    currentOffset = 0;
    for (auto &element : _childElements)
    {
      const UILayoutSettings &layoutSettings = element->GetLayoutSettings();
      Vector2 nextPosition = {0, 0};

      switch (layoutSettings.vAlignment)
      {
      case UIVAlignment::TOP:
        nextPosition.y = GetPosition().y + _padding.y;
        break;
      case UIVAlignment::CENTER:
        nextPosition.y = GetPosition().y + _baseSize.y / 2.f;
        nextPosition.y -= element->GetSize().y / 2;
        break;
      case UIVAlignment::BOTTOM:
        nextPosition.y = (GetPosition().y + _baseSize.y) - _padding.y - element->GetSize().y;
        break;
      }

      if (currentOffset == 0)
      {
        currentOffset += _padding.x;
        nextPosition.x = GetPosition().x + currentOffset;
      }
      else
      {
        currentOffset += _gapSize;
        nextPosition.x = GetPosition().x + currentOffset;
      }
      currentOffset += element->GetBaseSize().x;

      if (std::dynamic_pointer_cast<UIContainer>(element))
      {
        element->SetPosition(nextPosition);
      }
      else
      {
        element->_layoutPosition = nextPosition;
      }
    }
  }

  void UIContainer::UpdatePosition()
  {
    switch (_anchorPoint)
    {
    case TOP_LEFT:
      _layoutPosition = _basePosition;
      break;
    case TOP_CENTER:
      _layoutPosition.x = _basePosition.x - (_baseSize.x / 2);
      break;
    case TOP_RIGHT:
      _layoutPosition.x = _basePosition.x - _baseSize.x;
      break;
    case RIGHT_CENTER:
      _layoutPosition.x = _basePosition.x - _baseSize.x;
      _layoutPosition.y = _basePosition.y - (_baseSize.y / 2);
      break;
    case BOTTOM_RIGHT:
      _layoutPosition.x = _basePosition.x - _baseSize.x;
      _layoutPosition.y = _basePosition.y - _baseSize.y;
      break;
    case BOTTOM_CENTER:
      _layoutPosition.x = _basePosition.x - (_baseSize.x / 2);
      _layoutPosition.y = _basePosition.y - _baseSize.y;
      break;
    case BOTTOM_LEFT:
      _layoutPosition.y = _basePosition.y - _baseSize.y;
      break;
    case LEFT_CENTER:
      _layoutPosition.y = _basePosition.y - (_baseSize.y / 2);
      break;
    case CENTER:
      _layoutPosition.x = _basePosition.x - (_baseSize.x / 2);
      _layoutPosition.y = _basePosition.y - (_baseSize.y / 2);
      break;
    }
  }

  void UIContainer::OnInputEvent(std::shared_ptr<InputEvent> &event)
  {
    for (auto &element : _childElements)
    {
      if (element->IsVisible())
      {
        element->_onInputEvent(event);
      }
      if (event->isHandled)
      {
        break;
      }
    }
  }

  void UIContainer::SetAnchorPoint(AnchorPoint anchorPoint)
  {
    _anchorPoint = anchorPoint;
  }

  void UIContainer::SetBackgroundColor(Color color)
  {
    _backgroundColor = color;
  }

  Color UIContainer::GetBackgroundColor() const
  {
    return _backgroundColor;
  }

  void UIContainer::SetGapMode(UIContainer::GapMode gapmode)
  {
    _gapMode = gapmode;
  }

  void UIContainer::Render()
  {
    if (_sprite)
    {
      _sprite.Draw({GetPosition().x, GetPosition().y, GetSize().x, GetSize().y});
    }
    else
    {
      DrawRectangleRec({GetPosition().x, GetPosition().y, GetSize().x, GetSize().y}, _backgroundColor);
    }
    auto elements = std::ranges::reverse_view(_childElements);
    for (auto &element : elements)
    {
      if (element->IsVisible())
      {
        element->Render();
      }
    }
  }

  void UIContainer::Hide()
  {
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
