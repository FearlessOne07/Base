#include "base/entities/EntityManager.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/Entity.hpp"
#include "base/util/Exception.hpp"
#include "base/util/QuadTreeContainer.hpp"
#include <algorithm>
#include <memory>

namespace Base
{
  EntityManager::EntityManager()
  {
  }

  std::shared_ptr<Entity> EntityManager::CreateEntity()
  {
    if (_entityPending)
    {
      THROW_BASE_RUNTIME_ERROR(
        "Attempted Creation of entity before addition of previuously created one. Make Sure to call AddEntity()" //
      );
    }
    else
    {
      _entityPending = true;
      auto e = std::shared_ptr<Entity>(new Entity(_nextID++));
      e->AddComponent<TransformComponent>();
      return e;
    }
  };

  void EntityManager::UpdateEntity(std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator item)
  {
    ItemAreaType area;
    auto e = item->item;
    auto trancmp = e->GetComponent<TransformComponent>();
    if (e->HasComponent<ColliderComponent>())
    {
      auto colcmp = e->GetComponent<ColliderComponent>();
      if (colcmp->shape == ColliderComponent::Shape::CIRCLE)
      {
        area = Circle(trancmp->position, colcmp->radius);
      }
      else if (colcmp->shape == ColliderComponent::Shape::BOX)
      {
        area = Rectangle{
          trancmp->position.x - colcmp->positionOffset.x,
          trancmp->position.y - colcmp->positionOffset.y,
          colcmp->size.x,
          colcmp->size.y,
        };
      }
    }
    else
    {
      area = Rectangle{trancmp->position.x - 5, trancmp->position.y - 5, 10, 10};
    }
    _entities.UpdateItem(item, area);
  }

  void EntityManager::AddEntity(std::shared_ptr<Entity> e)
  {

    if (_entityPending)
    {
      _entityPending = false;
      ItemAreaType area;
      auto trancmp = e->GetComponent<TransformComponent>();
      if (e->HasComponent<ColliderComponent>())
      {
        auto colcmp = e->GetComponent<ColliderComponent>();
        if (colcmp->shape == ColliderComponent::Shape::CIRCLE)
        {
          area = Circle(trancmp->position, colcmp->radius);
        }
        else if (colcmp->shape == ColliderComponent::Shape::BOX)
        {
          area = Rectangle{
            trancmp->position.x - colcmp->positionOffset.x,
            trancmp->position.y - colcmp->positionOffset.y,
            colcmp->size.x,
            colcmp->size.y,
          };
        }
      }
      else
      {
        area = Rectangle{trancmp->position.x - 5, trancmp->position.y - 5, 10, 10};
      }
      _entities.Insert(e, area);
    }
  }

  const std::shared_ptr<Entity> EntityManager::GetEntity(EntityID id)
  {
    auto it = std::ranges::find_if(                                                              //
      _entities, [id](QuadTreeItem<std::shared_ptr<Entity>> e) { return e.item->GetID() == id; } //
    );
    return it != _entities.end() ? it->item : nullptr;
  }

  void EntityManager::Clear()
  {
    _entities.Clear();
    _nextID = 0;
  }

  template <>
  auto EntityManager::QueryArea<void>(ItemAreaType area)
    -> std::list<std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator>
  {
    return _entities.Search(area);
  }

  void EntityManager::RemoveDeadEntities()
  {
    for (auto it = _entities.begin(); it != _entities.end();)
    {
      if (!it->item->IsAlive())
      {
        _entities.Remove(it++);
        continue;
      }
      it++;
    }
  }

  void EntityManager::SetWorldBounds(Rectangle bounds)
  {
    _worldBounds = bounds;
    _entities.Clear();
    _entities = QuadTreeContainer<std::shared_ptr<Entity>>({_worldBounds});
  }

  const Rectangle EntityManager::GetWorldBounds() const
  {
    return _worldBounds;
  }
} // namespace Base
