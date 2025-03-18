#include "base/components/BoundingBoxComponent.hpp"

namespace Base
{
  void BoundingBoxComponent::SetTypeFlag(Type type)
  {
    _typeFlags.set(static_cast<uint8_t>(type));
  }

  bool BoundingBoxComponent::HasTypeFlag(Type type)
  {
    return _typeFlags.test(static_cast<uint8_t>(type));
  }
} // namespace Base
