#pragma once
#include "base/components/Component.hpp"
#include "base/entities/Entity.hpp"
#include <functional>
#include <vector>
namespace Base
{
  class ComponentBundle
  {
    using EntryFunction = std::function<void(std::shared_ptr<Base::Entity>)>;
    using ExitFunction = std::function<void(std::shared_ptr<Base::Entity>)>;

    std::vector<std::shared_ptr<Component>> _components;
    EntryFunction _entryFuntion = nullptr;
    ExitFunction _exitFunction = nullptr;

  public:
    template <typename... Components>
    ComponentBundle(EntryFunction entryFunction, ExitFunction exitFunction, Components &&...components)
      : _entryFuntion(entryFunction), _exitFunction(exitFunction)
    {
      (_components.push_back(components), ...);
    }
    ComponentBundle() = default;

    void AddComponents(std::shared_ptr<Base::Entity> entity);
    void RemoveComponents(std::shared_ptr<Base::Entity> entity);
  };
} // namespace Base
