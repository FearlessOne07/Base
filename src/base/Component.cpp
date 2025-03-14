#include "base/Component.hpp"
#include "base/Entity.hpp"
#include <memory>
#include <utility>

namespace Base
{
  Entity *Component::GetOwner()
  {
    return _owner.get();
  }

  void Component::SetOwner(std::shared_ptr<Entity> owner)
  {
    _owner = std::move(owner);
  }
} // namespace Base
