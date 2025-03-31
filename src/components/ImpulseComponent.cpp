#include "base/components/ImpulseComponent.hpp"
namespace Base
{
  bool ImpulseComponent::IsActive() const
  {
    return force != 0;
  }
} // namespace Base
