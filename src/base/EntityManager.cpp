#include "internal/entity/EntityManager.hpp"
#include <memory>

namespace Base
{
  std::shared_ptr<Entity> EntityManager::AddEntity()
  {
    _entities.push_back(std::shared_ptr<Entity>(new Entity(_nextID++)));
    return _entities.back();
  };
} // namespace Base
