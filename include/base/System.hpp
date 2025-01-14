#pragma once
#include "base/Entity.hpp"
#include <vector>

namespace Base
{

  class System
  {
  protected:
  public:
    virtual ~System()
    {
    }
    virtual void Update(float dt, std::vector<Entity> &entities) = 0;
  };

} // namespace Base
