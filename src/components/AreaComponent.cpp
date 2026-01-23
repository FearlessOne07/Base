#include "base/components/AreaComponent.hpp"
namespace Base
{
  AreaEntry::AreaEntry(Rectangle area) : _area(area)
  {
  }

  Rectangle AreaEntry::GetArea()
  {
    return _area;
  }

  AreaExit::AreaExit(Rectangle area) : _area(area)
  {
  }

  Rectangle AreaExit::GetArea()
  {
    return _area;
  }
} // namespace Base
