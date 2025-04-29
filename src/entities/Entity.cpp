#include "base/entities/Entity.hpp"
#include <cstddef>

namespace Base
{
  Entity::Entity(size_t id) : _id(id)
  {
  }

  Entity::Entity(Entity &&e) noexcept : _id(e._id), _components(std::move(e._components))
  {
    e._id = 0;
  }

  Entity &Entity::operator=(Entity &&e) noexcept
  {
    if (this != &e)
    {
      _id = e._id;
      _components = std::move(e._components);
      e._id = 0;
    }
    return *this;
  }

  size_t Entity::GetID() const
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
