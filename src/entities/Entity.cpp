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

  void Entity::RemoveComponent(const std::shared_ptr<Component> &component)
  {
    auto compID = std::type_index(typeid(*component));
    if (_components.find(compID) != _components.end())
    {
      _components.erase(compID);
    }
    else
    {
      std::stringstream error;
      error << "Failed to remove " << compID.name() << "; Entity[" << static_cast<int64_t>(_id)
            << "] does not have specified component\n";
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
  }
  void Entity::AddComponent(const std::shared_ptr<Component> &component)
  {
    // Get the actual type of the object pointed to
    auto compID = std::type_index(typeid(*component));

    // Check if component of this type already exists
    if (_components.find(compID) == _components.end())
    {
      _components[compID] = component;
      component->SetOwner(shared_from_this());
    }
    else
    {
      std::stringstream error;
      error << "Entity[" << static_cast<int64_t>(_id) << "] already has component " << compID.name();
      THROW_BASE_RUNTIME_ERROR(error.str());
    }
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
