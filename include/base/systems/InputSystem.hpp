#include "base/System.hpp"

namespace Base
{
  class   InputSystem : public System
  {
  private:
    void Start() override;
    void Stop() override;

  public:
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
