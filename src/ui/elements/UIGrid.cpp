#include "base/ui/elements/UIGrid.hpp"
#include "base/util/Draw.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <variant>

namespace Base
{
  void UIGrid::SetRowDefinitions(const std::vector<GridSizeUnit> &definitions)
  {
    _rowDefinitions = definitions;
    _rowSizes.resize(_rowDefinitions.size());
  }

  void UIGrid::SetColumnDefinitions(const std::vector<GridSizeUnit> &definitions)
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

      GridSizeUnit row = _rowDefinitions[position.x];
      GridSizeUnit column = _columnDefinitions[position.x];

      // Row
      if (std::holds_alternative<GridCellSizeMode>(row))
      {
        GridCellSizeMode mode = std::get<GridCellSizeMode>(row);

        if (mode == GridCellSizeMode::Auto)
        {
          _rowSizes[position.x] = std::max(csize.width, _rowSizes[position.x]);
        }
      }
      else if (std::holds_alternative<float>(row))
      {
        _rowSizes[position.x] = std::get<float>(row);
      }

      // Column
      if (std::holds_alternative<GridCellSizeMode>(column))
      {
        GridCellSizeMode mode = std::get<GridCellSizeMode>(column);

        if (mode == GridCellSizeMode::Auto)
        {
          _columnSizes[position.y] = std::max(csize.height, _columnSizes[position.y]);
        }
      }
      else if (std::holds_alternative<float>(column))
      {
        _columnSizes[position.y] = std::get<float>(column);
      }
    }

    _desiredSize.width = std::accumulate(_rowSizes.begin(), _rowSizes.end(), 0);
    _desiredSize.height = std::accumulate(_columnSizes.begin(), _columnSizes.end(), 0);
    return _desiredSize;
  }

  void UIGrid::Arrange(Rectangle finalRect)
  {

    _layoutRect.x = _desiredSize.width * _renderTransform.GetScaleX();
    _layoutRect.y = _desiredSize.height * _renderTransform.GetScaleY();
    _layoutRect.x += _renderTransform.GetOffsetx();
    _layoutRect.y += _renderTransform.GetOffsetY();
  }

  void UIGrid::Render(float alpha)
  {
    DrawRectangleBase(_layoutRect, {0, 0}, 0, GREEN);
  }
} // namespace Base
