#pragma once
#include "core/Entity.hpp"
#include <vector>

class System
{
public:
  virtual ~System()
  {
  }
  virtual void Update(float dt, std::vector<Entity> &entities) = 0;
};
