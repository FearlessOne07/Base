#pragma once
#include "Exports.hpp"
#include <memory>

namespace Base
{

  class Entity;
  struct BASEAPI Component
  {

    virtual ~Component() = default;

  public:
    Entity *GetOwner();
    void SetOwner(std::shared_ptr<Entity> owner);

  private:
    std::shared_ptr<Entity> _owner = nullptr;
  };

} // namespace Base
