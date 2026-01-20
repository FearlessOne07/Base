#include "base/ui/elements/UIGrid.hpp"
#include "base/ui/UIElement.hpp"
#include "internal/rendering/Renderer.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <ranges>
#include <variant>

namespace Base
{
  static bool operator==(Color a, Color b)
  {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
  }

  void UIGrid::SetRowDefinitions(const std::vector<GridDefinition> &definitions)
  {
    _rowDefinitions = definitions;
    _rowSizes.resize(_rowDefinitions.size());
  }

  void UIGrid::SetColumnDefinitions(const std::vector<GridDefinition> &definitions)
  {
    _columnDefinitions = definitions;
    _columnSizes.resize(_columnDefinitions.size());
  }

  Size UIGrid::Measure()
  {
    Size totalSize = {0, 0};
    for (auto it = _childElements.begin(); it != _childElements.end(); it++)
    {
      int index = std::distance(_childElements.begin(), it);
      GridPosition position = _elementGridPositions[index];
      Size csize = (*it)->Measure();

      GridSizeUnit column = _columnDefinitions[position.Column].Unit;
      GridSizeUnit row = _rowDefinitions[position.Row].Unit;

      // Row
      if (std::holds_alternative<GridCellSizeMode>(row))
      {
        GridCellSizeMode mode = std::get<GridCellSizeMode>(row);

        if (mode == GridCellSizeMode::Auto)
        {
          _rowSizes[position.Row] = std::max(csize.height, _rowSizes[position.Row]);
        }
        else
        {
          continue;
        }
      }
      else if (std::holds_alternative<float>(row))
      {
        _rowSizes[position.Row] = std::get<float>(row);
      }

      // Column
      if (std::holds_alternative<GridCellSizeMode>(column))
      {
        GridCellSizeMode mode = std::get<GridCellSizeMode>(column);

        if (mode == GridCellSizeMode::Auto)
        {
          _columnSizes[position.Column] = std::max(csize.width, _columnSizes[position.Column]);
        }
      }
      else if (std::holds_alternative<float>(column))
      {
        _columnSizes[position.Column] = std::get<float>(column);
      }
    }

    if (_widthSizeMode == SizeMode::Auto)
    {
      _desiredSize.width = std::accumulate(_columnSizes.begin(), _columnSizes.end(), 0);
      _desiredSize.width += _columnGap * (_columnDefinitions.size() - 1);
      _desiredSize.width += _paddingLeft + _paddingRight;
    }

    if (_heightSizeMode == SizeMode::Auto)
    {
      _desiredSize.height = std::accumulate(_rowSizes.begin(), _rowSizes.end(), 0);
      _desiredSize.height += _rowGap * (_rowDefinitions.size() - 1);
      _desiredSize.height += _paddingBottom + _paddingTop;
    }

    return _desiredSize;
  }

  void UIGrid::Arrange(Rectangle finalRect)
  {
    _layoutRect = finalRect;
    float width = _desiredSize.width * _renderTransform.GetScaleX();
    float height = _desiredSize.height * _renderTransform.GetScaleY();

    Vector2 finalPos = _layoutRect.GetPosition();
    Vector2 finalSize = {width, height};

    // Horizontal alignment
    switch (_horizontalAlignment)
    {
    case HAlign::Left:
      break;
    case HAlign::Center:
      finalPos.x += (finalRect.GetSize().x - width) / 2;
      break;
    case HAlign::Right:
      finalPos.x += finalRect.GetSize().x - width;
      break;
    case HAlign::Stretch:
      finalSize.x = finalRect.GetSize().x;
      break;
    }

    // Vertical alignment
    switch (_verticalAlignment)
    {
    case VAlign::Top:
      break;
    case VAlign::Center:
      finalPos.y += (finalRect.GetSize().y - height) / 2;
      break;
    case VAlign::Bottom:
      finalPos.y += finalRect.GetSize().y - height;
      break;
    case VAlign::Stretch:
      finalSize.y = finalRect.GetSize().y;
      break;
    }
    _layoutRect = {finalPos + Vector2{_renderTransform.GetOffsetx(), _renderTransform.GetOffsetY()}, finalSize};

    for (int index = 0; index < _childElements.size(); index++)
    {
      Rectangle childRect;
      auto &child = _childElements[index];
      GridPosition gridPos = _elementGridPositions[index];

      // Column Pos,
      float offset = _paddingLeft;
      Vector2 childPos;
      for (int i = 0; i < gridPos.Column; i++)
      {
        offset += _columnSizes[i];
      }
      childPos.x = offset + _layoutRect.GetPosition().x;

      offset = _paddingTop;
      for (int i = 0; i < gridPos.Row; i++)
      {
        offset += _rowSizes[i];
      }
      childPos.y = offset + _layoutRect.GetPosition().x;

      if (_elementGridPositions[index].Row != 0)
      {
        childPos.y += _rowGap;
      }

      if (_elementGridPositions[index].Column != 0)
      {
        childPos.x += _columnGap;
      }

      childRect = {childPos, {_columnSizes[gridPos.Column], _rowSizes[gridPos.Row]}};
      child->Arrange(childRect);
    }
  }

  void UIGrid::SetRowGap(float rowGap)
  {
    _rowGap = rowGap;
  }

  void UIGrid::SetColumnGap(float columnGap)
  {
    _columnGap = columnGap;
  }

  void UIGrid::SetCellGap(float cellGap)
  {
    _columnGap = cellGap;
    _rowGap = cellGap;
  }

  void UIGrid::Render(float opacity)
  {
    if (!_isHidden)
    {
      if (_sprite)
      {
        _sprite.Draw(_layoutRect, _renderTransform.GetOpacity() * opacity * 255);
      }
      else
      {
        float alpha = 0;
        if (_backgroundColor == Color{0, 0, 0, 0})
        {
          alpha = 0;
        }
        else
        {
          alpha = _renderTransform.GetOpacity();
        }
        Renderer::DrawQuad(_layoutRect, _layoutRect.GetPosition(), _backgroundColor);
      }

      auto elements = std::ranges::reverse_view(_childElements);
      for (auto &element : elements)
      {
        element->Render(_renderTransform.GetOpacity() * opacity);
      }
    }
  }
} // namespace Base
