#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/systems/System.hpp"
#include "base/util/QuadTreeContainer.hpp"
#include <memory>
#include <vector>

namespace Base
{
  class InputSystem : public System
  {
  private:
    std::vector<std::list<QuadTreeItem<std::shared_ptr<Entity>>>::iterator> _entities;
    Ref<EntityManager> _eMan;

  private:
    void OnKeyEvent(const std::shared_ptr<KeyEvent> &event);
    void OnMouseButtonEvent(const std::shared_ptr<MouseButtonEvent> &event);
    void OnInputEvent(std::shared_ptr<InputEvent> event) override;

  public:
    void Start() override;
    void Update(float dt, Ref<EntityManager> entityManager, std::shared_ptr<Scene> scene) override;
  };
} // namespace Base
