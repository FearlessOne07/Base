#include "base/components/ProximityComponent.hpp"

namespace Base
{

  ProximityEntry::ProximityEntry(EntityID radiusEntity, float radius) : _radiusEntity(radiusEntity), _radius(radius)
  {
  }

  float ProximityEntry::GetRadiusSq() const
  {
    return _radius * _radius;
  }

  EntityID ProximityEntry::GetRadiusEntity()
  {
    return _radiusEntity;
  }

  ProximityExit::ProximityExit(EntityID radiusEntity, float radius) : _radiusEntity(radiusEntity), _radius(radius)
  {
  }

  float ProximityExit::GetRadiusSq() const
  {
    return _radius * _radius;
  }

  EntityID ProximityExit::GetRadiusEntity()
  {
    return _radiusEntity;
  }
} // namespace Base
