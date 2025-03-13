#include "base/Component.hpp"

namespace Base
{
  Entity *Component::GetOwner()
  {
    return _owner.get();
  }
} // namespace Base
