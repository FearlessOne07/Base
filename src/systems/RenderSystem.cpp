
#include "base/systems/RenderSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/components/ABBComponent.hpp"
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
          DrawPolyLinesEx(mc->position, shc->points, shc->radius, shc->rotation, shc->nonFillThickness, shc->color);
        }
      }
    }

    // AABB Component
    std::vector<std::shared_ptr<Entity>> entities_abbcmp = entitymanager->Query<MoveComponent, ABBComponent>();
    for (std::shared_ptr<Entity> e : entities_abbcmp)
    {
      if (e)
      {
        ABBComponent *abbcmp = e->GetComponent<ABBComponent>();
        MoveComponent *mcmp = e->GetComponent<MoveComponent>();

        if (abbcmp && mcmp)
        {

          if (abbcmp->fill)
          {
            DrawRectanglePro( //
              {mcmp->position.x, mcmp->position.y, abbcmp->boundingBox.width, abbcmp->boundingBox.height}, {0, 0}, 0,
              abbcmp->color //
            );
          }
          else
          {
            DrawRectangleLinesEx(
              {mcmp->position.x, mcmp->position.y, abbcmp->boundingBox.width, abbcmp->boundingBox.height},
              abbcmp->nonFillThickness, abbcmp->color //
            );
          }
        }
      }
    }

    // Texture Component
    std::vector<std::shared_ptr<Entity>> entities_tcmp = entitymanager->Query<MoveComponent, TextureComponent>();
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
