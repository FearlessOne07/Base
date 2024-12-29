#pragma once
#include "base/Component.hpp"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <utility>

class Entity
{
  friend class EntityManager;

  size_t _id = 0;
  std::unordered_map<std::type_index, std::unique_ptr<Component>> _components;

  Entity(size_t id) : _id(id)
  {
  }

public:
  // Move constructor and Move assignment operator
  Entity(Entity &&e) noexcept : _id(e._id), _components(std::move(e._components))
  {
    e._id = 0;
  }

  Entity &operator=(Entity &&e) noexcept
  {
    if (this != &e)
    {
      _id = e._id;
      _components = std::move(e._components);
      e._id = 0;
    }
    return *this;
  }

  // Delete Copy comstructor and copy assignment operator
  Entity &operator=(Entity &) = delete;
  Entity(Entity &) = delete;

  template <typename T> bool HasComponent()
  {
    return _components.find(std::type_index(typeid(T))) == _components.end();
  }

  template <typename T, typename... Args> void AddComponent(Args &&...args)
  {
    std::type_index ti = std::type_index(typeid(T));

    if (!HasComponent<T>())
    {
      std::unique_ptr<Component> comp = std::make_unique<T>(std::forward<Args>(args)...);
      _components[ti] = std::move(comp);
    }
    else
    {
      throw std::runtime_error("Entity Already has compoment");
    }
  }

  template <typename T> T *GetComponent()
  {
    std::type_index ti = std::type_index(typeid(T));

    if (_components.find(ti) == _components.end())
    {
      std::stringstream error;
      error << "Failed to get component " << typeid(T).name() << "; Entity[" << _id
            << "] doesnot have specified component\n";
      throw std::runtime_error(error.str());
    }
    else
    {
      return static_cast<T *>(_components.at(ti).get());
    }
  }
};
