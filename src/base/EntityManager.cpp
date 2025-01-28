#include "base/EntityManager.hpp"
#include <memory>

namespace Base
{
  Entity *EntityManager::AddEntity()
  {
    _entities.push_back(std::shared_ptr<Entity>(new Entity(_nextID++)));
    return _entities.back().get();
  };
} // namespace Base
