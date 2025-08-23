#include "base/components/AreaComponent.hpp"
#include "raylib.h"
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
