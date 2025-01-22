
#include "base/systems/RenderSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TextureComponent.hpp"
#include "raylib.h"
#include <memory>
#include <vector>

namespace Base
{
  void RenderSystem::Update(float dt, EntityManager *entitymanager)
  {

    // Shape Component
    std::vector<std::shared_ptr<Entity>> entities_shcmp = entitymanager->Query<ShapeComponent, MoveComponent>();

    for (std::shared_ptr<Entity> e : entities_shcmp)
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

    // Texture Component
    std::vector<std::shared_ptr<Entity>> entities_tcmp = entitymanager->Query<ShapeComponent, MoveComponent>();
    for (std::shared_ptr<Entity> e : entities_tcmp)
    {
      if (e)
      {

        TextureComponent *tcmp = e->GetComponent<TextureComponent>();
        MoveComponent *mcmp = e->GetComponent<MoveComponent>();

        if (tcmp && mcmp)
        {
          DrawTexturePro( //
            tcmp->texture, {0, 0, tcmp->size.x, tcmp->size.y},
            {mcmp->position.x, mcmp->position.y, tcmp->size.x * tcmp->scale, tcmp->size.y * tcmp->scale}, tcmp->origin,
            0, WHITE //
          );
        }
      }
    }
  }
} // namespace Base
