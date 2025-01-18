#pragma once
#include "base/Entity.hpp"
#include "base/Exports.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace Base
{

  class BASEAPI EntityManager
  {
  private:
    std::vector<std::shared_ptr<Entity>> _entities;
    size_t _nextID = 0;

  public:
    std::shared_ptr<Entity> AddEntity();

    template <typename... Components> std::vector<std::shared_ptr<Entity>> Query()
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
