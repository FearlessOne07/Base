#pragma once
#include <memory>

namespace Base
{

  class Entity;
  class Component
  {
  public:
    virtual ~Component() = default;
    std::shared_ptr<Entity> GetOwner();
    void SetOwner(std::shared_ptr<Entity> owner);

  private:
    std::shared_ptr<Entity> _owner = nullptr;
  };

} // namespace Base
