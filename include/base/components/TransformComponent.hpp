#pragma once
#include "Component.hpp"
#include "base/util/Type.hpp"

namespace Base
{

  class TransformComponent : public Component
  {
  public:
    Vector2 position = {0, 0};
    float rotation = 0;
  };

} // namespace Base
