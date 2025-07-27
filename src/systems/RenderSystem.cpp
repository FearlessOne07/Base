#include "internal/systems/RenderSystem.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/SpriteComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/util/Draw.hpp"
#include "raylib.h"
#include <memory>

namespace Base
{
  void RenderSystem::Update(float dt, EntityManager *entitymanager, const Scene *currentScene)
  {
    // Shape Component
    auto entities_transcmp = entitymanager->Query<TransformComponent>();
    for (auto &item : entities_transcmp)
    {
      auto e = item->item;
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
              DrawRectangleBase( //
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
        }

        if (e->HasComponent<SpriteComponent>())
        {
          auto *sprtmp = e->GetComponent<SpriteComponent>();
          if (sprtmp && transcmp)
          {
            DrawTexturePro( //
              *sprtmp->GetTexture().Get()->GetRaylibTexture(), sprtmp->GetTextureSourceRect(),
              {transcmp->position.x, transcmp->position.y, sprtmp->GetTargetSize().x, sprtmp->GetTargetSize().y},
              {sprtmp->GetTargetSize().x / 2, sprtmp->GetTargetSize().y / 2}, transcmp->rotation, WHITE //
            );
          }
        }
      }
    }
  }
} // namespace Base
