#include "internal/entity/EntityManager.hpp"
#include <utility>

namespace Base
{
  Entity &EntityManager::AddEntity()
  {
    Entity e(_nextID++);
    _entities.emplace_back(std::move(e));
    return _entities.back();
  };
} // namespace Base