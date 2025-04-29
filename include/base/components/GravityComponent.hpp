#include "Component.hpp"

namespace Base
{
  struct GravityComponent : public Component
  {
    float gravityStrength = 0;
    float jumpHeight = 0;

    bool isJumping = false;
  };
} // namespace Base
