#pragma once
#include "base/Game.hpp"
namespace Base
{
  struct RenderContext;
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
