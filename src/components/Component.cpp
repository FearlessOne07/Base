#include "base/components/Component.hpp"
#include "base/entities/Entity.hpp"
#include <memory>
#include <utility>

namespace Base
{
  std::shared_ptr<Entity> Component::GetOwner()
  {
    return _owner;
  }

  void Component::SetOwner(std::shared_ptr<Entity> owner)
  {
    _owner = std::move(owner);
  }
} // namespace Base
