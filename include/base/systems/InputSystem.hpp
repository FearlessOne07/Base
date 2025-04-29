#include "System.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include <memory>
#include <vector>

namespace Base
{
  class InputSystem : public System
  {
  private:
    std::vector<std::shared_ptr<Entity>> _entities;

  private:
    void OnKeyEvent(const std::shared_ptr<KeyEvent> &event);
    void OnMouseButtonEvent(const std::shared_ptr<MouseButtonEvent> &event);

  public:
    void Start() override;
    void Update(float dt, EntityManager *entityManager) override;
  };
} // namespace Base
