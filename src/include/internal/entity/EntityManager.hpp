#pragma once
#include "base/Entity.hpp"
#include <cstddef>
#include <vector>

namespace Base
{

  class EntityManager
  {
  private:
    std::vector<Entity> _entities;
    size_t _nextID = 0;

  public:
    Entity &AddEntity();

    template <typename... Componets> std::vector<Base::Entity *> Query()
    {
      std::vector<Entity *> results = {};

      for (auto &e : _entities)
      {
        if ((e.HasComponent<Componets>() && ...))
        {
          results.push_back(&e);
        }
      }
      return results;
    }
  };
} // namespace Base
