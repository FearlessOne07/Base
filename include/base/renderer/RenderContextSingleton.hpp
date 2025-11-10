#pragma once
#include "base/game/Game.hpp"

namespace Base
{
  struct RenderContext
  {
    float gameWidth = 0.f;
    float gameHeight = 0.f;
    float marginX = 0.f;
    float marginY = 0.f;
    float scale = 0.f;
    Vector2 mousePosition = {0, 0};
  };

  class RenderContextSingleton
  {
  public:
    static const RenderContext *GetInstance();
    ~RenderContextSingleton();

  private:
    friend class Game;

    static void UpdateInstance(RenderContext *newContext);
    static RenderContext *_instance;
  };
} // namespace Base
