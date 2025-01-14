#pragma once
#include "base/Game.hpp"
namespace Base
{
  class RenderContext;
  class RenderContextSingleton
  {
  public:
    // Access the constant RenderContext
    static const RenderContext *GetInstance();

  private:
    friend class Game; // Allow `Game` to modify the RenderContext

    // Update the RenderContext (accessible only by `Game`)
    static void UpdateInstance(const RenderContext *newContext);
    static RenderContext _instance;
  };
} // namespace Base
