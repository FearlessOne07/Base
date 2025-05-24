#include "base/entities/Entity.hpp"
#include "base/signals/Signal.hpp"
#include "raylib.h"
#include <memory>

namespace Base
{
  class EntityCollisionSignal : public Signal
  {
  public:
    std::shared_ptr<Entity> hurtBoxEntity;
    std::shared_ptr<Entity> hittBoxEntity;
    Vector2 collisionNormal = {0, 0};
  };
} // namespace Base
