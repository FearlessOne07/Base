#pragma once

#include "base/components/TransitionComponent.hpp"
namespace Base
{
  class AreaEntry : public TransitionComponent
  {
  private:
    Rectangle _area{0, 0, 0, 0};

  public:
    AreaEntry(Rectangle area);
    Rectangle GetArea();
  };

  class AreaExit : public TransitionComponent
  {
  private:
    Rectangle _area{0, 0, 0, 0};

  public:
    AreaExit(Rectangle area);
    Rectangle GetArea();
  };
} // namespace Base
