#pragma once
#include "base/Component.hpp"
#include "base/Exports.hpp"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace Base
{

  class BASEAPI Entity : public std::enable_shared_from_this<Entity>
  {
    friend class EntityManager;

    size_t _id = 0;
    bool _alive = true;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> _components;
    Entity(size_t id);

  public:
    // Move constructor and Move assignment operator
    Entity(Entity &&e) noexcept;
    Entity &operator=(Entity &&e) noexcept;

    // Delete Copy comstructor and copy assignment operator
    Entity &operator=(Entity &) = delete;
    Entity(Entity &) = delete;

    template <typename T> bool HasComponent()
    {
      // Check Type of T
      static_assert(std::is_base_of_v<Component, T>, "T must derive from the class 'Component'");
      return _components.find(std::type_index(typeid(T))) != _components.end();
    }

    template <typename T> T *AddComponent()
    {
      // Check Type of T
      static_assert(std::is_base_of_v<Component, T>, "T must derive from the class 'Component'");
      auto ti = std::type_index(typeid(T));
      if (!HasComponent<T>())
      {
        std::unique_ptr<Component> comp = std::make_unique<T>();
        comp->SetOwner(shared_from_this());
        _components[ti] = std::move(comp);
        return static_cast<T *>(_components[ti].get());
      }
      else
      {
        throw std::runtime_error("Entity already has specified compoment");
      }
    }

    template <typename T> T *GetComponent() const
    {
      static_assert(std::is_base_of_v<Component, T>, "T must derive from the class 'Component'");

      auto ti = std::type_index(typeid(T));

      if (_components.find(ti) == _components.end())
      {
        std::stringstream error;
        error << "Failed to get component " << typeid(T).name() << "; Entity[" << _id
              << "] does not have specified component\n";
        throw std::runtime_error(error.str());
      }
      else
      {
        return static_cast<T *>(_components.at(ti).get());
      }
    }

    // Access
    size_t GetID() const;
    bool IsAlive() const;
    void SetDead();
  };
} // namespace Base
