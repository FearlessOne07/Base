#pragma once
#include "base/Game.hpp"
namespace Base
{
  class RenderContext;
  class   RenderContextSingleton
  {
  public:
    // Access the constant RenderContext
    static const RenderContext *GetInstance();
    ~RenderContextSingleton();

  private:
    friend class Game; // Allow `Game` to modify the RenderContext

    // Update the RenderContext (accessible only by `Game`)
    static void UpdateInstance(RenderContext *newContext);
    static RenderContext *_instance;
  };
} // namespace Base
