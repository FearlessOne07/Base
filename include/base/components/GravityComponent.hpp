#include "base/Component.hpp"

namespace Base
{
  struct GravityComponent : public Component
  {
    float gravityStrength = 0;
    float jumpHeight = 0;

    // State
    bool isJumping = false;
  };
} // namespace Base
