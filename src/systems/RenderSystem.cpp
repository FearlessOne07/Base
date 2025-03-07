
#include "base/systems/RenderSystem.hpp"
#include "base/EntityManager.hpp"
#include "base/components/BoundingBoxComponent.hpp"
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
    for (std::shared_ptr<Entity> &e : entities_transcmp)
    {
      if (e)
      {
        auto *transcmp = e->GetComponent<TransformComponent>();

        if (e->HasComponent<ShapeComponent>())
        {
          auto *shc = e->GetComponent<ShapeComponent>();

          if (shc->fill)
          {
            DrawPoly(transcmp->position, shc->points, shc->radius, transcmp->rotation, shc->color);
          }
          else
          {
            DrawPolyLinesEx(                                                                                      //
              transcmp->position, shc->points, shc->radius, transcmp->rotation, shc->nonFillThickness, shc->color //
            );
          }
        }

        if (e->HasComponent<BoundingBoxComponent>())
        {
          auto *abbcmp = e->GetComponent<BoundingBoxComponent>();

          if (abbcmp->draw)
          {
            if (abbcmp->fill)
            {
              DrawRectanglePro( //
                {abbcmp->lastPosition.x, abbcmp->lastPosition.y, abbcmp->size.x, abbcmp->size.y}, {0, 0},
                transcmp->rotation,
                RED //
              );
              DrawRectanglePro( //
                {transcmp->position.x, transcmp->position.y, abbcmp->size.x, abbcmp->size.y}, {0, 0},
                transcmp->rotation,
                abbcmp->color //
              );
            }
            else
            {
              DrawRectangleLinesEx(                                                               //
                {abbcmp->lastPosition.x, abbcmp->lastPosition.y, abbcmp->size.x, abbcmp->size.y}, //
                abbcmp->nonFillThickness, RED                                                     //
              );
              DrawRectangleLinesEx(                                                           //
                {transcmp->position.x, transcmp->position.y, abbcmp->size.x, abbcmp->size.y}, //
                abbcmp->nonFillThickness, abbcmp->color                                       //
              );
            }
          }
        }
        else if (e->HasComponent<TextureComponent>())
        {
          auto *tcmp = e->GetComponent<TextureComponent>();

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
    std::vector<std::shared_ptr<Entity>> entities_tcmp_abb =
      entitymanager->Query<BoundingBoxComponent, TextureComponent>();
    for (std::shared_ptr<Entity> &e : entities_tcmp_abb)
    {
      if (e)
      {
        auto *tcmp = e->GetComponent<TextureComponent>();
        auto *abbcmp = e->GetComponent<BoundingBoxComponent>();
        auto *transcmp = e->GetComponent<TransformComponent>();

        if (tcmp && abbcmp && !abbcmp->draw)
        {
          DrawTexturePro( //
            *tcmp->texture, tcmp->source,
            {transcmp->position.x, transcmp->position.y, tcmp->source.width * tcmp->scale,
             tcmp->source.height * tcmp->scale},
            tcmp->origin, 0,
            WHITE //
          );
        }
      }
    }
  }
} // namespace Base
