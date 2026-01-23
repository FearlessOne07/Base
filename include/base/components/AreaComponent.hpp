#pragma once

#include "base/components/TransitionComponent.hpp"
#include "base/rendering/Quad.hpp"
namespace Base
{
  class AreaEntry : public TransitionComponent
  {
  private:
    Rectangle _area;

  public:
    AreaEntry(Rectangle area);
    Rectangle GetArea();
  };

  class AreaExit : public TransitionComponent
  {
  private:
    Rectangle _area;

  public:
    AreaExit(Rectangle area);
    Rectangle GetArea();
  };
} // namespace Base
