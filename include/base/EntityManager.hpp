#pragma once
#include "base/Entity.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace Base
{
  class EntityManager
  {
  private:
    friend class Game;
    std::vector<std::shared_ptr<Entity>> _entities;
    size_t _nextID = 0;

    void RemoveDeadEntities();
    void Update();

  public:
    EntityManager();
    Entity *AddEntity();
    const Entity *GetEntity(size_t id);

    template <typename... Components> auto Query() -> std::vector<std::shared_ptr<Entity>>
    {
      std::vector<std::shared_ptr<Entity>> results = {};

      for (auto &e : _entities)
      {
        if ((e->HasComponent<Components>() && ...))
        {
          results.emplace_back(e);
        }
      }
      return results;
    }
  };

} // namespace Base
