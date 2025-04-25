#include "base/components/ColliderComponent.hpp"

namespace Base
{
  void ColliderComponent::SetTypeFlag(Type type)
  {
    _typeFlags.set(static_cast<uint8_t>(type));
  }

  bool ColliderComponent::HasTypeFlag(Type type)
  {
    return _typeFlags.test(static_cast<uint8_t>(type));
  }
} // namespace Base
