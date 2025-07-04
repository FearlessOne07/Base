#include "base/ui/elements/UIContainer.hpp"
#include "raylib.h"
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
        nextPosition.x = _currentPosition.x + _padding.x;
        break;
      case UIHAlignment::CENTER:
        nextPosition.x = _currentPosition.x + _baseSize.x / 2.f;
        nextPosition.x -= element->GetSize().x / 2;
        break;
      case UIHAlignment::RIGHT:
        nextPosition.x = (_currentPosition.x + _baseSize.x) - _padding.x - element->GetSize().x;
        break;
      }

      if (currentOffset == 0)
      {
        currentOffset += _padding.y;
        nextPosition.y = _currentPosition.y + currentOffset;
      }
      else
      {
        currentOffset += _gapSize;
        nextPosition.y = _currentPosition.y + currentOffset;
      }
      currentOffset += element->GetBaseSize().y;
      element->SetPosition(nextPosition, false);
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
        nextPosition.y = _currentPosition.y + _padding.y;
        break;
      case UIVAlignment::CENTER:
        nextPosition.y = _currentPosition.y + _baseSize.y / 2.f;
        nextPosition.y -= element->GetSize().y / 2;
        break;
      case UIVAlignment::BOTTOM:
        nextPosition.y = (_currentPosition.y + _baseSize.y) - _padding.y - element->GetSize().y;
        break;
      }

      if (currentOffset == 0)
      {
        currentOffset += _padding.x;
        nextPosition.x = _currentPosition.x + currentOffset;
      }
      else
      {
        currentOffset += _gapSize;
        nextPosition.x = _currentPosition.x + currentOffset;
      }
      currentOffset += element->GetBaseSize().x;
      element->SetPosition(nextPosition, false);
    }
  }

  void UIContainer::UpdatePosition()
  {
    switch (_anchorPoint)
    {
    case TOP_LEFT:
      break;
    case TOP_CENTER:
      _currentPosition.x = _basePosition.x - (_baseSize.x / 2);
      break;
    case TOP_RIGHT:
      _currentPosition.x = _basePosition.x - _baseSize.x;
      break;
    case RIGHT_CENTER:
      _currentPosition.x = _basePosition.x - _baseSize.x;
      _currentPosition.y = _basePosition.y - (_baseSize.y / 2);
      break;
    case BOTTOM_RIGHT:
      _currentPosition.x = _basePosition.x - _baseSize.x;
      _currentPosition.y = _basePosition.y - _baseSize.y;
      break;
    case BOTTOM_CENTER:
      _currentPosition.x = _basePosition.x - (_baseSize.x / 2);
      _currentPosition.y = _basePosition.y - _baseSize.y;
      break;
    case BOTTOM_LEFT:
      _currentPosition.y = _basePosition.y - _baseSize.y;
      break;
    case LEFT_CENTER:
      _currentPosition.y = _basePosition.y - (_baseSize.y / 2);
      break;
    case CENTER:
      _currentPosition.x = _basePosition.x - (_baseSize.x / 2);
      _currentPosition.y = _basePosition.y - (_baseSize.y / 2);
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

  void UIContainer::SetGapMode(UIContainer::GapMode gapmode)
  {
    _gapMode = gapmode;
  }

  void UIContainer::Render()
  {
    auto elements = std::ranges::reverse_view(_childElements);
    for (auto &element : elements)
    {
      element->Render();
    }

    DrawRectangleLinesEx({_currentPosition.x, _currentPosition.y, _baseSize.x, _baseSize.y}, 3, RED);
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
