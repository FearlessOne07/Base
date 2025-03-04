#include "base/EntityManager.hpp"
#include "base/components/TransformComponent.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace Base
{
  EntityManager::EntityManager()
  {
    _entities.reserve(100);
  }

  Entity *EntityManager::AddEntity()
  {
    _entities.push_back(std::shared_ptr<Entity>(new Entity(_nextID++)));
    _entities.back()->AddComponent<TransformComponent>();
    return _entities.back().get();
  };

  const Entity *EntityManager::GetEntity(size_t id)
  {
    auto it = std::ranges::find_if(_entities, [id](std::shared_ptr<Entity> &e) { return e->GetID() == id; });
    return it != _entities.end() ? it->get() : nullptr;
  }

  void EntityManager::RemoveDeadEntities()
  {
    auto dead = std::ranges::remove_if(_entities, [](std::shared_ptr<Entity> &e) { //
      return !(e->IsAlive());
    });
    _entities.erase(dead.begin(), dead.end());
    std::cout << _entities.size() << "\n";
  }
} // namespace Base
