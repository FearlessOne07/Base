#pragma once 
#include <memory>

namespace Base
{

  class Entity;
  struct   Component
  {

    virtual ~Component() = default;

  public:
    Entity *GetOwner();
    void SetOwner(std::shared_ptr<Entity> owner);

  private:
    std::shared_ptr<Entity> _owner = nullptr;
  };

} // namespace Base
