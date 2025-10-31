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
    std::vector<GridSizeUnit> _rowDefinitions = {};
    std::vector<GridSizeUnit> _columnDefinitions = {};
    std::vector<Vector2> _elementGridPositions = {};
    std::vector<float> _rowSizes = {};
    std::vector<float> _columnSizes = {};

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

      gridPosition.x = std::trunc(gridPosition.x);
      gridPosition.y = std::trunc(gridPosition.y);

      if (gridPosition.x >= _rowDefinitions.size() || gridPosition.y >= _columnDefinitions.size())
      {
        THROW_BASE_RUNTIME_ERROR("Grid Position Greater than available rows or columns");
      }

      if (gridPosition.x <= 0 && gridPosition.y <= 0)
      {
        THROW_BASE_RUNTIME_ERROR("Invalid Grid Position");
      }

      _elementGridPositions.push_back(gridPosition);
      return AddChild<T>(name);
    }
  };
} // namespace Base
