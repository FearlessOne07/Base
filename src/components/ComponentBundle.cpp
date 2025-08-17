#include "base/components/ComponentBundle.hpp"
namespace Base
{
  void ComponentBundle::AddComponents(std::shared_ptr<Base::Entity> entity)
  {
    for (auto &comp : _components)
    {
      entity->AddComponent(comp);
    }
    _entryFuntion(entity);
  }

  void ComponentBundle::RemoveComponents(std::shared_ptr<Base::Entity> entity)
  {
    _exitFunction(entity);
    for (auto &comp : _components)
    {
      entity->RemoveComponent(comp);
    }
  }

} // namespace Base
