#pragma once
#include "Entity.hpp"
#include "base/util/QuadTree.hpp"
#include "base/util/QuadTreeContainer.hpp"
#include "raylib.h"
#include <cstddef>
#include <list>
#include <memory>
#include <vector>

namespace Base
{
  class EntityManager
  {
  private:
    friend class Game;
    QuadTreeContainer<std::shared_ptr<Entity>> _entities{{0, 0, 0, 0}};
    Rectangle _worldBounds = {0, 0};
    size_t _nextID = 0;
    bool _entityPending = false;

    void RemoveDeadEntities();
    void Update();

  public:
    EntityManager();
    std::shared_ptr<Entity> CreateEntity();
    void AddEntity(std::shared_ptr<Entity>);
    void UpdateEntity(std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator item);

    const std::shared_ptr<Entity> GetEntity(size_t id);
    void SetWorldBounds(Rectangle bounds);
    const Rectangle GetWorldBounds() const;
    std::list<std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator> QueryArea(ItemAreaType area);

    template <typename... Components>
    auto Query() -> std::vector<std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator>
    {
      std::vector<std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator> results = {};
      for (auto it = _entities.begin(); it != _entities.end(); it++)
      {
        if ((it->item->HasComponent<Components>() && ...) && it->item->IsAlive())
        {
          results.emplace_back(it);
        }
      }
      return results;
    }
  };

} // namespace Base
