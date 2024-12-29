#pragma once
#include "core/Entity.hpp"
#include <cstddef>
#include <vector>

class EntityManager
{
private:
  std::vector<Entity> _entities;
  size_t _nextID = 0;

public:
  Entity &AddEntity();
};
