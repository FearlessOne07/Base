#include "base/systems/RenderSystem.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TextureComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
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

        if (e->HasComponent<ColliderComponent>())
        {
          auto *abbcmp = e->GetComponent<ColliderComponent>();

          if (abbcmp->draw)
          {
            if (abbcmp->fill)
            {
              DrawRectanglePro( //
                {transcmp->position.x - abbcmp->positionOffset.x, transcmp->position.y - abbcmp->positionOffset.y,
                 abbcmp->size.x, abbcmp->size.y},
                {0, 0}, transcmp->rotation,
                abbcmp->color //
              );
            }
            else
            {
              DrawRectangleLinesEx( //
                {
                  transcmp->position.x - abbcmp->positionOffset.x,
                  transcmp->position.y - abbcmp->positionOffset.y,
                  abbcmp->size.x,
                  abbcmp->size.y,
                },                                      //
                abbcmp->nonFillThickness, abbcmp->color //
              );
            }
          }

          if (e->HasComponent<TextureComponent>())
          {
            auto *tcmp = e->GetComponent<TextureComponent>();

            if (tcmp && transcmp)
            {
              DrawTexturePro( //
                *tcmp->texture, tcmp->source,
                {
                  transcmp->position.x,
                  transcmp->position.y,
                  abbcmp->size.x,
                  abbcmp->size.y,
                },
                abbcmp->positionOffset, transcmp->rotation, WHITE //
              );
            }
          }
        }
      }
    }
  }
} // namespace Base
