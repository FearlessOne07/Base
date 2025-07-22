#include "base/entities/Entity.hpp"
#include <cstddef>

namespace Base
{
  EntityID::operator bool()
  {
    return _id >= 0;
  }

  EntityID::operator int64_t() const
  {
    return _id;
  }

  bool EntityID::operator==(const EntityID &other)
  {
    return _id == other._id;
  }

  EntityID::EntityID(int64_t id)
  {
    _id = id;
  }

  EntityID::EntityID()
  {
    _id = -1;
  }

  Entity::Entity(size_t id) : _id(static_cast<int64_t>(id))
  {
  }

  Entity::Entity(Entity &&e) noexcept : _id(static_cast<int64_t>(e._id)), _components(std::move(e._components))
  {
    e._id = EntityID(-1);
  }

  Entity &Entity::operator=(Entity &&e) noexcept
  {
    if (this != &e)
    {
      _id = e._id;
      _components = std::move(e._components);
      e._id = EntityID(-1);
    }
    return *this;
  }

  EntityID Entity::GetID() const
  {
    return _id;
  }

  void Entity::SetDead()
  {
    _alive = false;
  }

  bool Entity::IsAlive() const
  {
    return _alive;
  }

} // namespace Base
