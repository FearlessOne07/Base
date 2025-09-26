#pragma once
#include "base/components/Component.hpp"
#include "base/util/Exception.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <sstream>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace Base
{
  class EntityID
  {
  private:
    int64_t _id = -1;
    friend class Entity;

  private:
    explicit EntityID(int64_t id);

  public:
    EntityID();
    operator bool();
    operator int64_t() const;
    bool operator==(const EntityID &other);
  };

  class Entity : public std::enable_shared_from_this<Entity>
  {
    friend class EntityManager;

    EntityID _id;
    bool _alive = true;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> _components;
    Entity(size_t id);

  public:
    Entity(Entity &&e) noexcept;
    Entity &operator=(Entity &&e) noexcept;

    // Delete Copy comstructor and copy assignment operator
    Entity &operator=(Entity &) = delete;
    Entity(Entity &) = delete;

    template <typename T>
      requires(std::is_base_of_v<Component, T>)
    bool HasComponent() const
    {
      return _components.find(std::type_index(typeid(T))) != _components.end();
    }

    void RemoveComponent(const std::shared_ptr<Component> &component);
    template <typename T>
      requires(std::is_base_of_v<Component, T>)
    void RemoveComponent()
    {
      auto compID = std::type_index(typeid(T));
      if (HasComponent<T>())
      {
        _components.erase(compID);
      }
      else
      {
        std::stringstream error;
        error << "Failed to get remove " << typeid(T).name() << "; Entity[" << static_cast<int64_t>(_id)
              << "] does not have specified component\n";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
    }

    void AddComponent(const std::shared_ptr<Component> &component);
    template <typename T, typename... Args>
      requires(std::is_base_of_v<Component, T>)
    std::shared_ptr<T> AddComponent(Args &&...args)
    {
      auto compID = std::type_index(typeid(T));

      if (!HasComponent<T>())
      {
        std::shared_ptr<Component> comp = std::make_shared<T>(std::forward<Args>(args)...);
        comp->SetOwner(shared_from_this());
        _components[compID] = std::move(comp);
        return static_pointer_cast<T>(_components[compID]);
      }
      else
      {
        std::stringstream error;
        error << "Entity[" << static_cast<int64_t>(_id) << "] already has compoment " << typeid(T).name();
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
    }

    template <typename T>
      requires(std::is_base_of_v<Component, T>)
    std::shared_ptr<T> GetComponent() const
    {
      auto compId = std::type_index(typeid(T));
      if (_components.find(compId) == _components.end())
      {
        std::stringstream error;
        error << "Failed to get component " << typeid(T).name() << "; Entity[" << static_cast<int64_t>(_id)
              << "] does not have specified component\n";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      else
      {
        return static_pointer_cast<T>(_components.at(compId));
      }
    }

    // Access
    EntityID GetID() const;
    bool IsAlive() const;
    void SetDead();
  };
} // namespace Base
