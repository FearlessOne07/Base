#include "base/Entity.hpp"
#include "base/Event.hpp"
#include <memory>

namespace Base
{
  class EntityCollisionEvent : public Event
  {
  public:
    std::shared_ptr<Entity> hurtBoxEntity;
    std::shared_ptr<Entity> hittBoxEntity;
  };
} // namespace Base
