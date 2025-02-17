#include "base/EntityManager.hpp"
#include <algorithm>
#include <memory>

namespace Base
{
  Entity *EntityManager::AddEntity()
  {
    _entities.push_back(std::shared_ptr<Entity>(new Entity(_nextID++)));
    return _entities.back().get();
  };

  const Entity *EntityManager::GetEntity(size_t id)
  {
    auto it =
      std::find_if(_entities.begin(), _entities.end(), [id](std::shared_ptr<Entity> &e) { return e->GetID() == id; });
    return it->get();
  }
} // namespace Base
