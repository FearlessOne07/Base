#include "core/entity/EntityManager.hpp"
#include <utility>

Entity &EntityManager::AddEntity()
{
  Entity e(_nextID++);
  _entities.emplace_back(std::move(e));
  return _entities.back();
};
