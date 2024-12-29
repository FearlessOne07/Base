#pragma once
#include "RenderContext.hpp"
#include "base/Game.hpp"

class RenderContextSingleton
{
public:
  // Access the constant RenderContext
  static const RenderContext &GetInstance()
  {
    return _instance;
  }

private:
  friend class Game; // Allow `Game` to modify the RenderContext

  // Update the RenderContext (accessible only by `Game`)
  static void UpdateInstance(const RenderContext &newContext)
  {
    _instance = newContext;
  }

  static RenderContext _instance; // The shared RenderContext instance
};
