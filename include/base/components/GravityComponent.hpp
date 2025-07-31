#include "Component.hpp"

namespace Base
{
  class GravityComponent : public Component
  {
  public:
    float gravityStrength = 0;
    float jumpHeight = 0;

    bool isJumping = false;
  };
} // namespace Base
