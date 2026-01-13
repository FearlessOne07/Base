#pragma once

#include "Component.hpp"
#include "base/util/Type.hpp"

namespace Base
{
  class ImpulseComponent : public Component
  {
  public:
    Vector2 direction = {0, 0};
    float force = 0;

    bool IsActive() const;
  };
} // namespace Base
