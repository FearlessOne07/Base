#pragma once

#include "base/components/TransitionComponent.hpp"
#include "base/entities/Entity.hpp"
namespace Base
{
  class ProximityEntry : public TransitionComponent
  {
  private:
    EntityID _radiusEntity;
    float _radius;

  public:
    ProximityEntry(EntityID radiusEntity, float radius);
    float GetRadiusSq() const;
    EntityID GetRadiusEntity();
  };

  class ProximityExit : public TransitionComponent
  {
  private:
    float _radius;
    EntityID _radiusEntity;

  public:
    ProximityExit(EntityID radiusEntity, float radius);
    float GetRadiusSq() const;
    EntityID GetRadiusEntity();
  };
} // namespace Base
