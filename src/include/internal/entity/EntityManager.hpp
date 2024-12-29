#pragma once
#include "base/Entity.hpp"
#include <cstddef>
#include <vector>

namespace Base
{

class EntityManager
{
private:
  std::vector<Entity> _entities;
  size_t _nextID = 0;

public:
  Entity &AddEntity();
};
} // namespace Base
