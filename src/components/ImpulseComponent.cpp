#include "base/components/ImpulseComponent.hpp"
namespace Base
{
  bool ImpulseComponent::IsActive() const
  {
    return elapsedTime < duration;
  }
} // namespace Base
