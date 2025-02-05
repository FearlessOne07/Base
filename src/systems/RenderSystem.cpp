
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

        if (abbcmp && mcmp && abbcmp->draw)
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
            *tcmp->texture, tcmp->source,
            {mcmp->position.x, mcmp->position.y, tcmp->source.width * tcmp->scale, tcmp->source.height * tcmp->scale},
            tcmp->origin, 0, WHITE //
          );
        }
      }
    }

    std::vector<std::shared_ptr<Entity>> entities_tcmp_abb = entitymanager->Query<ABBComponent, TextureComponent>();
    for (std::shared_ptr<Entity> e : entities_tcmp_abb)
    {
      if (e)
      {
        TextureComponent *tcmp = e->GetComponent<TextureComponent>();
        ABBComponent *abbcmp = e->GetComponent<ABBComponent>();

        if (tcmp && abbcmp && !abbcmp->draw)
        {
          DrawTexturePro( //
            *tcmp->texture, tcmp->source,
            {abbcmp->boundingBox.x, abbcmp->boundingBox.y, tcmp->source.width * tcmp->scale,
             tcmp->source.height * tcmp->scale},
            tcmp->origin, 0,
            WHITE //
          );
        }
      }
    }
  }
} // namespace Base
