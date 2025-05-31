#include "base/renderer/RenderContextSingleton.hpp"

namespace Base
{
  RenderContext *RenderContextSingleton::_instance = nullptr;

  const RenderContext *RenderContextSingleton::GetInstance()
  {
    if (_instance)
    {
      return _instance;
    }
    else
    {
      _instance = new RenderContext;
      return _instance;
    }
  }

  void RenderContextSingleton::UpdateInstance(RenderContext *newContext)
  {
    _instance = newContext;
  }

  // Destructor: cleans up the instance if it exists
  RenderContextSingleton::~RenderContextSingleton()
  {
    if (_instance)
    {
      delete _instance; // Clean up the instance if it was allocated
      _instance = nullptr;
    }
  }
} // namespace Base
