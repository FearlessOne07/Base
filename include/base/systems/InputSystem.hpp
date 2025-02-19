#include "base/Exports.hpp"
#include "base/System.hpp"

namespace Base
{
  class BASEAPI InputSystem : public System
  {
  public:
    virtual void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
