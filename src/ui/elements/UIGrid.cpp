#include "base/ui/elements/UIGrid.hpp"
#include "base/ui/UIElement.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"
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
      Vector2 position = _elementGridPositions[index];
      Size csize = (*it)->Measure();

      GridSizeUnit column = _columnDefinitions[position.x].Unit;
      GridSizeUnit row = _rowDefinitions[position.y].Unit;

      // Row
      if (std::holds_alternative<GridCellSizeMode>(row))
      {
        GridCellSizeMode mode = std::get<GridCellSizeMode>(row);

        if (mode == GridCellSizeMode::Auto)
        {
          _rowSizes[position.y] = std::max(csize.height, _rowSizes[position.y]);
        }
        else
        {
          continue;
        }
      }
      else if (std::holds_alternative<float>(row))
      {
        _rowSizes[position.y] = std::get<float>(row);
      }

      // Column
      if (std::holds_alternative<GridCellSizeMode>(column))
      {
        GridCellSizeMode mode = std::get<GridCellSizeMode>(column);

        if (mode == GridCellSizeMode::Auto)
        {
          _columnSizes[position.x] = std::max(csize.width, _columnSizes[position.x]);
        }
      }
      else if (std::holds_alternative<float>(column))
      {
        _columnSizes[position.x] = std::get<float>(column);
      }
    }

    if (_widthSizeMode == SizeMode::Auto)
    {
      _desiredSize.width = std::accumulate(_columnSizes.begin(), _columnSizes.end(), 0);
    }
    else
    {
    }

    if (_heightSizeMode == SizeMode::Auto)
    {
      _desiredSize.height = std::accumulate(_rowSizes.begin(), _rowSizes.end(), 0);
    }
    return _desiredSize;
  }

  void UIGrid::Arrange(Rectangle finalRect)
  {
    _layoutRect = finalRect;
    float width = _desiredSize.width * _renderTransform.GetScaleX();
    float height = _desiredSize.height * _renderTransform.GetScaleY();

    // Horizontal alignment
    switch (_horizontalAlignment)
    {
    case HAlign::Left:
      break;
    case HAlign::Center:
      _layoutRect.x += (finalRect.width - width) / 2;
      break;
    case HAlign::Right:
      _layoutRect.x += finalRect.width - width;
      break;
    case HAlign::Stretch:
      width = finalRect.width;
      break;
    }

    // Vertical alignment
    switch (_verticalAlignment)
    {
    case VAlign::Top:
      break;
    case VAlign::Center:
      _layoutRect.y += (finalRect.height - height) / 2;
      break;
    case VAlign::Bottom:
      _layoutRect.y += finalRect.height - height;
      break;
    case VAlign::Stretch:
      height = finalRect.height;
      break;
    }

    _layoutRect.width = std::min(width, finalRect.width);
    _layoutRect.height = std::min(height, finalRect.height);

    _layoutRect.x += _renderTransform.GetOffsetx();
    _layoutRect.y += _renderTransform.GetOffsetY();

    for (int index = 0; index < _childElements.size(); index++)
    {
      Rectangle childRect = {0, 0, 0, 0};
      auto &child = _childElements[index];
      Vector2 gridPos = _elementGridPositions[index];

      // Column Pos,
      float offset = 0;
      for (int i = 0; i < gridPos.x; i++)
      {
        offset += _columnSizes[i];
      }
      childRect.x = offset + _layoutRect.x;

      offset = 0;
      for (int i = 0; i < gridPos.y; i++)
      {
        offset += _rowSizes[i];
      }
      childRect.y = offset + _layoutRect.y;

      childRect.width = _columnSizes[gridPos.x];
      childRect.height = _rowSizes[gridPos.y];

      child->Arrange(childRect);
    }
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
        if (_backgroundColor == BLANK)
        {
          alpha = 0;
        }
        else
        {
          alpha = _renderTransform.GetOpacity();
        }
        DrawRectangleBase( //
          {_layoutRect.x, _layoutRect.y, _layoutRect.width, _layoutRect.height}, {0, 0}, 0,
          {
            _backgroundColor.r,
            _backgroundColor.g,
            _backgroundColor.b,
            static_cast<unsigned char>(alpha * opacity * 255),
          } //
        );
      }

      auto elements = std::ranges::reverse_view(_childElements);
      for (auto &element : elements)
      {
        element->Render(_renderTransform.GetOpacity() * opacity);
      }
    }
  }
} // namespace Base
