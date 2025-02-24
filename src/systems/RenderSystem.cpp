
#include "base/systems/RenderSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/components/ABBComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TextureComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib.h"
#include <memory>
#include <vector>

namespace Base
{
  void RenderSystem::Update(float dt, EntityManager *entitymanager)
  {
    // Shape Component
    std::vector<std::shared_ptr<Entity>> entities_transcmp = entitymanager->Query<TransformComponent>();
    for (std::shared_ptr<Entity> e : entities_transcmp)
    {
      if (e)
      {
        TransformComponent *transcmp = e->GetComponent<TransformComponent>();

        if (e->HasComponent<ShapeComponent>())
        {
          ShapeComponent *shc = e->GetComponent<ShapeComponent>();

          if (shc->fill)
          {
            DrawPoly(transcmp->position, shc->points, shc->radius, transcmp->rotation, shc->color);
          }
          else
          {
            DrawPolyLinesEx(transcmp->position, shc->points, shc->radius, transcmp->rotation, shc->nonFillThickness,
                            shc->color);
          }
        }
        else if (e->HasComponent<ABBComponent>())
        {
          ABBComponent *abbcmp = e->GetComponent<ABBComponent>();

          if (abbcmp && transcmp && abbcmp->draw)
          {
            if (abbcmp->fill)
            {
              DrawRectanglePro( //
                {transcmp->position.x, transcmp->position.y, abbcmp->boundingBox.width, abbcmp->boundingBox.height},
                {0, 0}, transcmp->rotation,
                abbcmp->color //
              );
            }
            else
            {
              DrawRectangleLinesEx(
                {transcmp->position.x, transcmp->position.y, abbcmp->boundingBox.width, abbcmp->boundingBox.height},
                abbcmp->nonFillThickness, abbcmp->color //
              );
            }
          }
        }
        else if (e->HasComponent<TextureComponent>())
        {
          TextureComponent *tcmp = e->GetComponent<TextureComponent>();

          if (tcmp && transcmp)
          {
            DrawTexturePro( //
              *tcmp->texture, tcmp->source,
              {transcmp->position.x, transcmp->position.y, tcmp->source.width * tcmp->scale,
               tcmp->source.height * tcmp->scale},
              tcmp->origin, transcmp->rotation, WHITE //
            );
          }
        }
      }
    }

    // ABB + Texture
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
