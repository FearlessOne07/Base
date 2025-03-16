#include "base/Exports.hpp"
#include "base/System.hpp"

namespace Base
{
  class BASEAPI InputSystem : public System
  {
  private:
    void Start() override;
    void Stop() override;

  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
