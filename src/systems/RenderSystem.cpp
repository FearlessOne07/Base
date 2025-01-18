
#include "base/systems/RenderSystem.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "internal/entity/EntityManager.hpp"
#include "raylib.h"
#include <memory>
#include <vector>
namespace Base
{
  void RenderSystem::Update(float dt, EntityManager *entitymanager)
  {
    std::vector<std::shared_ptr<Entity>> entities = entitymanager->Query<ShapeComponent, MoveComponent>();

    for (std::shared_ptr<Entity> e : entities)
    {
      if (e)
      {
        ShapeComponent *shc = e->GetComponent<ShapeComponent>();
        MoveComponent *mc = e->GetComponent<MoveComponent>();

        if (shc->fill)
        {
          DrawPoly(mc->position, shc->points, shc->radius, shc->rotation, shc->color);
        }
        else
        {
          DrawPolyLines(mc->position, shc->points, shc->radius, shc->rotation, shc->color);
        }
      }
    }
  }
} // namespace Base
