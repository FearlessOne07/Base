#pragma once
#include "base/System.hpp"

namespace Base
{
  class MoveSystem : public System
  {
  public:
    void Update(float dt, std::vector<Entity> &entities) override;
  };

} // namespace Base
