#include "base/ui/UIElement.hpp"
#include "base/util/Exception.hpp"
#include "raylib.h"
#include <cmath>
#include <variant>

namespace Base
{
  enum GridCellSizeMode
  {
    Fill,
    Auto
  };

  using GridSizeUnit = std::variant<float, GridCellSizeMode>;
  class UIGrid : public UIElement
  {
  private:
    std::vector<GridSizeUnit> _rowDefinitions = {GridCellSizeMode::Auto};
    std::vector<GridSizeUnit> _columnDefinitions = {GridCellSizeMode::Auto};
    std::vector<Vector2> _elementGridPositions = {};
    std::vector<float> _rowSizes = {0};
    std::vector<float> _columnSizes = {0};

    // Color
    Color _backgroundColor = GREEN;

  public:
    void SetRowDefinitions(const std::vector<GridSizeUnit> &definitions);
    void SetColumnDefinitions(const std::vector<GridSizeUnit> &definitions);

    Size Measure() override;
    void Arrange(Rectangle finalRect) override;

    void Render(float alpha) override;

    template <typename T>
      requires(std::is_base_of_v<UIElement, T>)
    std::shared_ptr<T> AddGridElement(const std::string &name, Vector2 gridPosition)
    {
      // Round down to nearest whole grid cell
      gridPosition.x = std::trunc(gridPosition.x);
      gridPosition.y = std::trunc(gridPosition.y);

      // Grid bounds check (zero-indexed)
      if (gridPosition.y < 0 || gridPosition.y >= static_cast<float>(_rowDefinitions.size()))
      {
        THROW_BASE_RUNTIME_ERROR("Grid row position out of bounds");
      }

      if (gridPosition.x < 0 || gridPosition.x >= static_cast<float>(_columnDefinitions.size()))
      {
        THROW_BASE_RUNTIME_ERROR("Grid column position out of bounds");
      }

      _elementGridPositions.push_back(gridPosition);
      return AddChild<T>(name);
    }
  };
} // namespace Base
