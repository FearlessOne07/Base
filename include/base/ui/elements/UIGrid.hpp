#include "base/ui/UIElement.hpp"
#include "base/util/Exception.hpp"
#include "raylib.h"
#include <cmath>
#include <variant>

namespace Base
{
  enum GridCellSizeMode
  {
    Auto,
    Star
  };

  using GridSizeUnit = std::variant<float, GridCellSizeMode>;

  struct GridDefinition
  {
    GridDefinition(const GridSizeUnit &unit) : Unit(unit)
    {
    }

    GridDefinition(const GridSizeUnit &unit, int scale) : Unit(unit), Scale(scale)
    {
    }

    GridSizeUnit Unit;
    int Scale = 1;
  };

  struct GridPosition
  {
    int Column = 0;
    int Row = 0;
  };

  class UIGrid : public UIElement
  {
  private:
    std::vector<GridDefinition> _rowDefinitions = {{GridCellSizeMode::Auto}};
    std::vector<GridDefinition> _columnDefinitions = {{GridCellSizeMode::Auto}};
    std::vector<GridPosition> _elementGridPositions = {};
    std::vector<float> _rowSizes = {0};
    std::vector<float> _columnSizes = {0};

    // Color
    Color _backgroundColor = GREEN;

  public:
    void SetRowDefinitions(const std::vector<GridDefinition> &definitions);
    void SetColumnDefinitions(const std::vector<GridDefinition> &definitions);

    Size Measure() override;
    void Arrange(Rectangle finalRect) override;

    void Render(float alpha) override;

    template <typename T>
      requires(std::is_base_of_v<UIElement, T>)
    std::shared_ptr<T> AddGridElement(const std::string &name, GridPosition gridPosition)
    {
      // Round down to nearest whole grid cell
      gridPosition.Column = std::trunc(gridPosition.Column);
      gridPosition.Row = std::trunc(gridPosition.Row);

      // Grid bounds check (zero-indexed)
      if (gridPosition.Row < 0 || gridPosition.Row >= static_cast<float>(_rowDefinitions.size()))
      {
        THROW_BASE_RUNTIME_ERROR("Grid row position out of bounds");
      }

      if (gridPosition.Column < 0 || gridPosition.Column >= static_cast<float>(_columnDefinitions.size()))
      {
        THROW_BASE_RUNTIME_ERROR("Grid column position out of bounds");
      }

      _elementGridPositions.push_back(gridPosition);
      return AddChild<T>(name);
    }
  };
} // namespace Base
