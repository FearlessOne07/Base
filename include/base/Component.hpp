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

  private:
    std::shared_ptr<Entity> _owner = nullptr;
  };

} // namespace Base
