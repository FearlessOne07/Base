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

  public:
    explicit EntityID(int64_t id);
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
    std::unordered_map<std::type_index, std::unique_ptr<Component>> _components;
    Entity(size_t id);

  public:
    Entity(Entity &&e) noexcept;
    Entity &operator=(Entity &&e) noexcept;

    // Delete Copy comstructor and copy assignment operator
    Entity &operator=(Entity &) = delete;
    Entity(Entity &) = delete;

    template <typename T> bool HasComponent() const
    {
      // Check if T is a derivative of Base::Component
      static_assert(std::is_base_of_v<Component, T>, "T must derive from the class 'Component'");

      // Check wether is exsits in the _components map an return the result
      return _components.find(std::type_index(typeid(T))) != _components.end();
    }

    template <typename T, typename... Args> T *AddComponent(Args &&...args)
    {
      // Check if T is a derivative of Base::Component
      static_assert(std::is_base_of_v<Component, T>, "T must derive from the class 'Component'");

      // Generate Component ID
      auto compID = std::type_index(typeid(T));

      // Check if component alreadt exists  on entity
      if (!HasComponent<T>())
      {
        // Create Component
        std::unique_ptr<Component> comp = std::make_unique<T>(std::forward<Args>(args)...);

        // Set The component Owner
        comp->SetOwner(shared_from_this());

        // Move component to map
        _components[compID] = std::move(comp);

        // Return Component for setup
        return static_cast<T *>(_components[compID].get());
      }
      else
      {
        THROW_BASE_RUNTIME_ERROR("Entity already has specified compoment");
      }
    }

    template <typename T> T *GetComponent() const
    {
      // Check if T is a derivative of Base::Component
      static_assert(std::is_base_of_v<Component, T>, "T must derive from the class 'Component'");

      auto compId = std::type_index(typeid(T));

      // Check if component exists
      if (_components.find(compId) == _components.end())
      {
        std::stringstream error;
        error << "Failed to get component " << typeid(T).name() << "; Entity[" << static_cast<int64_t>(_id)
              << "] does not have specified component\n";
        THROW_BASE_RUNTIME_ERROR(error.str());
      }
      else
      {
        // if so, return a pointer to it
        return static_cast<T *>(_components.at(compId).get());
      }
    }

    // Access
    EntityID GetID() const;
    bool IsAlive() const;
    void SetDead();
  };
} // namespace Base
